#ifndef DVIDROPAREA_H
#define DVIDROPAREA_H

#include <QLabel>
#include <QUrl>

class DVIDropArea : public QLabel
{
    Q_OBJECT
  public:
    DVIDropArea();
    DVIDropArea(QWidget* parent) : QLabel(parent) {}

  protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent* event) override;

  signals:
    void droppedArchive(QString path, bool noTest);
};

#endif // DVIDROPAREA_H
