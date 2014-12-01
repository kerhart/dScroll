#include "dscrollbar.h"

#include <fstream>
#include <iostream>

#include <QTextBlock>
#include <qtconcurrentrun.h>
//#include <QTextLayout>
//#include <QTextDocumentFragment>

#include <QMessageBox>

#include <texteditor/basetexteditor.h>
#include <texteditor/basetextdocumentlayout.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorsettings.h>

using namespace dScroll::Internal;

dScrollBar::dScrollBar(TextEditor::BaseTextEditorWidget* edit)
    : myTextEditor(edit), QScrollBar(), myWidth(150), myIsRepainting(false), myNeedsRepainting(false), myFutureFinished(false)
{
//    this->setStyleSheet(QLatin1String("background: red"));

    myFont = TextEditor::TextEditorSettings::instance()->fontSettings().font();

    double parentFullHeight = computeHeight(myTextEditor->document()->lineCount(), myFont);
    int extraAreaWidth = myTextEditor->extraAreaWidth();
    int scrollBarWidth = width();
    myPixmap = QPixmap(myTextEditor->width() - extraAreaWidth - scrollBarWidth, parentFullHeight);
    myPixmap.fill(TextEditor::TextEditorSettings::instance()->fontSettings().formatFor(TextEditor::C_TEXT).background());

//    myFont.setPixelSize(3);

    myRenderFutureWatcher = new QFutureWatcher<QPixmap>(this);
    connect(myRenderFutureWatcher, SIGNAL(finished()), this, SLOT(pixmapUpdateComplete()));
}

//dScrollBar::dScrollBar(TextEditor::BaseTextEditorWidget* edit)
//    : m_parentEdit(edit)
//{
//    m_parentEdit->viewport()->installEventFilter(this);

//    connect(m_parentEdit, SIGNAL(textChanged()), SLOT(documentContentChanged()));
//    connect(m_parentEdit, SIGNAL(selectionChanged()), SLOT(documentSelectionChanged()));

//    // Catch notifications about code folding.
//    connect(
//        m_parentEdit->document()->documentLayout(),
//        SIGNAL(documentSizeChanged(const QSizeF&)),
//        SLOT(onDocumentSizeChanged(const QSizeF&)));

//    m_refreshTimer.setSingleShot(true);
//    connect(&m_refreshTimer, SIGNAL(timeout()), SLOT(onRefreshTimeout()));

//    m_refreshTimer.start(REFRESH_PERIOD_MSEC);
//}

double dScrollBar::computeHeight(int lineCount, QFont const &font) const
{
    QFontMetricsF fm(font);

    double documentHeight = fm.height() * lineCount;
    double lineLeading = fm.leading();
    if (lineCount > 1 && lineLeading > 0.0)
    {
        // Add line spacing.
        documentHeight += (lineCount - 1) * lineLeading;
    }
    return documentHeight;
}

//int dScrollBar::computeVisibleLines() const
//{
//    return myTextEditor->document()->lineCount();
//}

void dScrollBar::drawWindowRect(QPainter& p, qreal startY, qreal sizeY)
{
    QPointF viewportTopLeft(0, startY);

    QRectF rect(viewportTopLeft, QSizeF(myWidth, sizeY));

    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(0, 0, 255, 50)));
    p.drawRect(rect);
}

QSize dScrollBar::minimumSizeHint() const
{
    return QSize(myWidth, 0);
}

void dScrollBar::mouseMoveEvent(QMouseEvent *event)
{
    if(myIsLeftButtonPressed)
    {
        setValue(posToScrollValue(event->posF().y()));
    }
}

void dScrollBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setValue(posToScrollValue(event->posF().y()));
        myIsLeftButtonPressed = true;
    }
    else if(event->button() == Qt::RightButton)
    {
//        clearHighlight();
        update();
    }
}

void dScrollBar::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        myIsLeftButtonPressed = false;
    }
}

int dScrollBar::onScreenLineCount() const
{
    int lineCount = 0;
    QTextBlock textBlock = myTextEditor->document()->firstBlock();
    while (textBlock.isValid())
    {
        if (!textBlock.userData() || !static_cast<TextEditor::TextBlockUserData*>(textBlock.userData())->folded())
        {
            lineCount += textBlock.lineCount();
        }
        else
        {
            // Folded rows are shown on a single line.
            lineCount += 1;
        }
        textBlock = textBlock.next();
    }
    return lineCount - maximum();
}

void dScrollBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

//    QMessageBox::information(this, QLatin1String("Paint Event"), QString(QLatin1String("Here A %1")).arg(myIsRepainting));

    if (myIsRepainting) { myNeedsRepainting = true; return; }

//    QMessageBox::information(this, QLatin1String("Paint Event"), QString(QLatin1String("Here B %1")).arg(myFutureFinished));

//    double parentFullHeight = computeHeight(myTextEditor->document()->lineCount());
//    int extraAreaWidth = 0;
//    int scrollBarWidth = width();
//    myPixmap = QPixmap(myTextEditor->width() - extraAreaWidth - scrollBarWidth, parentFullHeight);
//    myPixmap.fill(fontSettings.formatFor(TextEditor::C_TEXT).background());
//    myTextEditor->render(&myPixmap, QPoint(), QRegion(extraAreaWidth, 0, myTextEditor->width() - extraAreaWidth - scrollBarWidth, parentFullHeight) );
    if (!myFutureFinished) // && !myRenderFutureWatcher->isRunning())
    {
        myIsRepainting = true;
        myRenderFutureWatcher->setFuture(QtConcurrent::run(renderToPixmap, this));
//        renderToPixmap();
    }
    else
    {
        myFutureFinished = false;

        double scrollValue = value();

        updateGeometry();
//        myPixmap = renderToPixmap(this);

        double refHeight = computeHeight(myTextEditor->document()->lineCount(), myFont);
        mySqueezeFactorY = height() / refHeight;

        setValue(scrollValue);

        QPainter p(this);
        TextEditor::FontSettings fontSettings = TextEditor::TextEditorSettings::instance()->fontSettings();

        // Draw background to match editor
        p.fillRect(rect(), fontSettings.formatFor(TextEditor::C_TEXT).background());

        // Draw the pixmap
        p.drawPixmap(0, 0, width(), height(), myPixmap);

        // Draw visible window section
        drawWindowRect(p, computeHeight(value(), myFont) * mySqueezeFactorY, computeHeight(onScreenLineCount(), myFont) * mySqueezeFactorY);

        p.end();

        if (myNeedsRepainting)
        {
            myNeedsRepainting = false;
//            this->repaint();
            paintEvent(event);
        }
    }
}

void dScrollBar::pixmapUpdateComplete()
{
    myIsRepainting = false;
    myFutureFinished = true;
    myPixmap = static_cast<QPixmap>(myRenderFutureWatcher->future().result());
    this->repaint();
}

int dScrollBar::posToScrollValue(qreal pos) const
{
    qreal documentHeight = computeHeight(myTextEditor->document()->lineCount(), myFont) * mySqueezeFactorY;
    int scrollTick = int(pos * (maximum() + onScreenLineCount()) / documentHeight);

    // set center of a viewport to position of click
    scrollTick -= onScreenLineCount() / 2;
    if(scrollTick > maximum())
    {
        scrollTick = maximum();
    }
    else if (scrollTick < minimum())
    {
        scrollTick = minimum();
    }
    return scrollTick;
}

QSize dScrollBar::sizeHint() const
{
    return QSize(myWidth, 0);
}

QPixmap dScroll::Internal::renderToPixmap(dScrollBar *sbar)
{
    std::ofstream stream;
    stream.open ("C:\\TempR\\FileOut.txt", std::ofstream::out | std::ofstream::app);
    double parentFullHeight = sbar->computeHeight(sbar->myTextEditor->document()->lineCount(), TextEditor::TextEditorSettings::instance()->fontSettings().font());
    int extraAreaWidth = sbar->myTextEditor->extraAreaWidth();
    int scrollBarWidth = sbar->width();
    QPixmap pixmap(sbar->myTextEditor->width() - extraAreaWidth - scrollBarWidth, parentFullHeight);
    pixmap.fill(TextEditor::TextEditorSettings::instance()->fontSettings().formatFor(TextEditor::C_TEXT).background());

    stream << "A: " << parentFullHeight << " " << extraAreaWidth << " " << scrollBarWidth << " " << sbar->myTextEditor->width() << '\n';
    stream << "B: " << pixmap.size().height() << " " << pixmap.width() << '\n';
    stream.close();

    QSize origSize = sbar->myTextEditor->size();
    sbar->myTextEditor->resize(QSize(origSize.width(), parentFullHeight));
//    sbar->myTextEditor->render(&pixmap, QPoint(), QRegion(extraAreaWidth, 0, sbar->myTextEditor->width() - extraAreaWidth - scrollBarWidth, parentFullHeight));
    sbar->myTextEditor->resize(origSize);
    return pixmap.scaled(scrollBarWidth, sbar->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    return pm;
}


































