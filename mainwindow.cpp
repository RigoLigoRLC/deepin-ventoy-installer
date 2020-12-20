#include "mainwindow.h"

#include <QCloseEvent>
#include <QAction>
#include <QMenu>
#include <DMainWindow>
#include <DDialog>
#include <DTitlebar>

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    w = new Widget;

    resize(w->size()); //设置窗口大小
    setCentralWidget(w);
    initWindow();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
  if(w->hasOngoingOperation())
  {
    DDialog askExit;
    askExit.setMessage(tr("There is still ongoing tasks running!\n"
                          "Terminating the program right now could cause problems.\n\n"
                          "Continue?"));
    askExit.setTitle(tr("Exit"));
    askExit.setIcon(QIcon::fromTheme("dialog-warning"));
    askExit.addButtons({ tr("Yes"), tr("No") });
    if(askExit.exec() == 0)
    {
      w->forceStopAll();
      e->accept();
    }
    else
    {
      e->ignore();
    }
  }
  else
  {
    e->accept();
  }
}

void MainWindow::initWindow()
{
  actGoDropArchive = new QAction(this);
  actGoDropArchive->setText(tr("Go back to drop archive"));
  connect(actGoDropArchive, &QAction::triggered, w, &Widget::goBackDropArchive);
  QMenu *menu = new QMenu(this->titlebar());
  menu->addAction(actGoDropArchive);
  this->titlebar()->setMenu(menu);
  this->titlebar()->setIcon(QIcon(":/images/res/icon.png"));
}
