#pragma once
#ifndef DSCROLLBAR_H
#define DSCROLLBAR_H

#include <QFutureWatcher>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QPixmap>

namespace TextEditor
{
    class BaseTextEditorWidget;
}


namespace dScroll {
namespace Internal {

class dScrollBar : public QScrollBar
{
    Q_OBJECT

public:

    dScrollBar(TextEditor::BaseTextEditorWidget *edit);

//    void markStateDirty() { m_stateDirty = true; }
//    void fullUpdateSettings();

protected:

    QSize minimumSizeHint() const;

    void paintEvent(QPaintEvent *event);

    QSize sizeHint() const;

//    void resizeEvent(QResizeEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

//    void contextMenuEvent(QContextMenuEvent *event);

//    inline const CoolScrollbarSettings& settings() const { return *m_settings; }

//    QSize sizeHint() const;
//    QSize minimumSizeHint() const;

//    int unfoldedLinesCount() const;
//    int linesInViewportCount() const;
//    qreal calculateLineHeight() const;
//    qreal lineCountToDocumentHeight(int lineCount) const;

//    // original document access
//    const QTextDocument& originalDocument() const;
//    QTextDocument& originalDocument();

//    bool eventFilter(QObject *obj, QEvent *e);

protected slots:

    void pixmapUpdateComplete();

//    void documentContentChanged();
//    void documentSelectionChanged();
//    void onRefreshTimeout();
//    void onDocumentSizeChanged(const QSizeF& newSize);

private:

    double computeHeight(int lineCount, const QFont &font) const;

//    int computeVisibleLines() const;

//    void drawSelections(QPainter& p);

    void drawWindowRect(QPainter& p, qreal startY, qreal sizeY);

    int onScreenLineCount() const;

    int posToScrollValue(qreal pos) const;

//    void renderToPixmap();

//    void drawFoldingMarkers(QPainter& p);

//    void restartDeferredUpdate();
//    void updatePicture();

//    int posToScrollValue(qreal pos) const;

//    void highlightSelectedWord();
//    void clearHighlight();
//    void highlightEntryInDocument(const QString& str);

//    qreal documentHeightVirtual() const;
//    qreal documentHeightScreen() const;

//    TextEditor::BaseTextEditorWidget* m_parentEdit;
//    const QSharedPointer<CoolScrollbarSettings> m_settings;

//    QPixmap m_previewPic;
//    QVector<QRectF> m_selectionRects;

//    QString m_stringToHighlight;

//    bool m_highlightNextSelection;
//    bool m_leftButtonPressed;

//    bool m_stateDirty;

//    bool m_redrawOnResize;

//    QTimer m_refreshTimer;

//    /// Relative factor between the virtual document and the screen space X.
//    qreal m_squeezeFactorX;

//    /// Relative factor between the virtual document and the screen space Y.
//    qreal m_squeezeFactorY;

    TextEditor::BaseTextEditorWidget* myTextEditor;
    QPixmap myPixmap;

    bool myIsRepainting;
    bool myNeedsRepainting;
    bool myFutureFinished;
    QFutureWatcher<QPixmap> *myRenderFutureWatcher;

    double mySqueezeFactorY;

    bool myIsLeftButtonPressed;

    // Settings
    int myWidth;
    QFont myFont;

    friend QPixmap renderToPixmap(dScrollBar *);
};

QPixmap renderToPixmap(dScrollBar *sbar);

}
}

#endif // DSCROLLBAR_H
