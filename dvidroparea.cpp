#include "dvidroparea.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>

DVIDropArea::DVIDropArea()
{

}

void DVIDropArea::dragEnterEvent(QDragEnterEvent *event)
{
  auto mimeData = event->mimeData();
  if(mimeData->hasUrls())
  {
    const QString &mimeStr = mimeData->text();

    // Restrict to local .tar.gz files only
    if(mimeStr.right(7) == ".tar.gz" && mimeStr.left(7) == "file://")
    {
      event->acceptProposedAction();
    }
  }
}

void DVIDropArea::dragMoveEvent(QDragMoveEvent *event)
{
  event->acceptProposedAction();
}

void DVIDropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
  event->accept();
}

void DVIDropArea::dropEvent(QDropEvent *event)
{
  auto mimeData = event->mimeData();
  const QString &mimeStr = mimeData->text();
  QFileInfo fileInfo;
  bool noTest = false;
  if(event->keyboardModifiers().testFlag(Qt::KeyboardModifier::ShiftModifier))
  {
    noTest = true;
  }
  if(mimeStr.indexOf('\n') == -1) // Only one file
  {
    fileInfo.setFile(QUrl(mimeStr).path());
    if(fileInfo.isFile())
    {
      emit droppedArchive(mimeData->text(), noTest);
      event->acceptProposedAction();
    }
  }
}
