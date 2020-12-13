#include "widget.h"
#include "ui_widget.h"
#include "utilities.h"

#include <DMessageBox>
#include <DGuiApplicationHelper>
#include <QProcess>
#include <QStorageInfo>

#include <iostream>

using namespace Dtk::Gui;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
  ui->setupUi(this);
  initWidget();
}

Widget::~Widget()
{
  delete ui;
}

void Widget::processVentoyArchive(QString aPath)
{
  unarchiveProcess->kill();
  unarchiveProcess->waitForFinished();
  if(!tmpDir.removeRecursively())
  {
    qWarning() << "Failed to remove the temporary directory for extraction";
  }
  tmpDir.mkpath(tmpDir.path());
  unarchiveProcess->start(QString("tar -xavf %1 -C %2").arg(QUrl(aPath).path()).arg(tmpDir.path()));
}

void Widget::finishedUnarchive()
{
  switch(unarchiveProcess->exitCode())
  {
    case 0:
      break;
    case 1:
      qInfo() << "Extracted files differ; they have been updated.";
      break;
    default:
      QMessageBox failExtractMsg;
      failExtractMsg.setText(tr("Extraction of the selected archive was not successful; exit code: %1.")
                             .arg(unarchiveProcess->exitCode()));
      failExtractMsg.setIcon(QMessageBox::Critical);
      failExtractMsg.setDetailedText(unarchiveProcess->readAllStandardError());
      failExtractMsg.exec();
      return;
  }
  tmpDir.cd(tmpDir.path());

  // Successfully unarchived, let's roll!
  QDir::setCurrent(tmpDir.path());

  QString dviWorkDir = findDVIWorkFile(tmpDir);
  if(dviWorkDir == "")
  {
    QMessageBox failFindScriptMsg;
    failFindScriptMsg.setText(tr("No working Ventoy build can be found in the extracted archive."));
    failFindScriptMsg.setIcon(QMessageBox::Critical);
    failFindScriptMsg.exec();
    return;
  }

  QDir::setCurrent(dviWorkDir);

  switchToInstaller();
}

void Widget::proceedInstallation()
{
  Dtk::Widget::DMessageBox confirm;
  confirm.setText(tr("You're about to install Ventoy into %1. Confirm?\n"
                     "ALL your data on the target will be erased!")
                  .arg(ui->selectDevice_combo->currentData().toString()));
  confirm.addButton(QMessageBox::StandardButton::Ok);
  confirm.addButton(QMessageBox::StandardButton::Cancel);
  if(confirm.exec() == QMessageBox::StandardButton::Ok)
  {
    confirm.setText(tr("Double-check! The target device is %1.\n"
                       "Installing Ventoy will destroy ALL DATA present on the device!")
                    .arg(ui->selectDevice_combo->currentData().toString()));
    if(confirm.exec() == QMessageBox::StandardButton::Ok)
    {
      qInfo() << "Install.";
    }
  }
}

void Widget::changeTheme(bool aDark)
{
  if(aDark)
  {
    ui->btnRefreshDeviceList->setIcon(QIcon(":/images/res/refresh-dark.svg"));
    ui->btnQueryDeviceVentoyVer->setIcon(QPixmap(":/images/res/refresh-dark.svg"));
    ui->lblPictureDropArchive->setPixmap(QPixmap(":/images/res/drop-dark.svg"));
  }
  else
  {
    ui->btnRefreshDeviceList->setIcon(QIcon(":/images/res/refresh.svg"));
    ui->btnQueryDeviceVentoyVer->setIcon(QPixmap(":/images/res/refresh.svg"));
    ui->lblPictureDropArchive->setPixmap(QPixmap(":/images/res/drop.svg"));
  }
}

void Widget::queryDeviceList()
{
  QProcess deviceQueryProc;
  deviceQueryProc.start("lsblk -d -o PATH,SIZE");
  deviceQueryProc.waitForFinished(); // This shouldn't take too long so it's running blocking

  if(deviceQueryProc.exitCode())
  {
    QMessageBox errorMsgBox;
    errorMsgBox.setText(tr("Cannot query available devices; exit code: %1.")
                        .arg(deviceQueryProc.exitCode()));
    errorMsgBox.setDetailedText(deviceQueryProc.readAllStandardError());
    errorMsgBox.exec();
    return;
  }

  ui->selectDevice_combo->clear();

  QString deviceQueryResult = deviceQueryProc.readAllStandardOutput();
  QStringList devicePathList = deviceQueryResult.split("\n");
  devicePathList.removeFirst(); // First line would be "PATH"
  devicePathList.removeLast(); // Last line would be empty, there is an '\n' in ouput at last
  foreach (auto &&i, devicePathList)
  {
    ui->selectDevice_combo->addItem(i, i.section(' ', 0, 0));
  }
}

void Widget::switchToInstaller()
{
  emit getArchiveVentoyVersion();
  emit queryDeviceList();
  ui->mainPager->setCurrentIndex(1);
}

void Widget::verifyVentoyFeatures()
{
  enableCheckBox(m_archiveVer >= SemanticVersion(1, 0, 28), ui->chkUseCustomVolLabel);
  enableCheckBox(m_archiveVer >= SemanticVersion(1, 0, 15), ui->chkUseGptPartTable);
  enableCheckBox(m_archiveVer >= SemanticVersion(1, 0, 7), ui->chkEnableSecureBoot);
}

void Widget::getArchiveVentoyVersion()
{
  QFile ventoyVersion("ventoy/version");
  if(ventoyVersion.open(QFile::ReadWrite))
  {
    QString version = ventoyVersion.readAll();
    version.chop(1);
    ui->lblSelectedVentoyVerVal->setText(version);
    m_archiveVer = version;
    verifyVentoyFeatures();
    ventoyVersion.close();
  }
}

void Widget::getDeviceVentoyVersion()
{
  QProcess getDeviceVer;
  bool forceExit = false;
  getDeviceVer.start("sh");
  getDeviceVer.waitForStarted();
  getDeviceVer.write(". tool/ventoy_lib.sh\n");
  getDeviceVer.write(QString("tool/vtoyfat_32 `get_disk_part_name %1 2`\n")
                     .arg(ui->selectDevice_combo->currentData().toString()).toStdString().data());
  getDeviceVer.write("exit\n");
  if(!getDeviceVer.waitForFinished(100))
  {
    forceExit = true;
    getDeviceVer.kill();
    getDeviceVer.waitForFinished();
  }
  qInfo() << getDeviceVer.exitCode() << forceExit << QString(getDeviceVer.readAllStandardError());
  if(!getDeviceVer.exitCode() && !forceExit)
  {
    QString deviceVer = getDeviceVer.readAll();
    deviceVer.chop(1);
    ui->lblDeviceVentoyVerVal->setText(deviceVer);
    m_DeviceVer = deviceVer;
  }
  else
  {
    ui->lblDeviceVentoyVerVal->setText("N/A");
    m_DeviceVer = SemanticVersion(0, 0, 0);
  }
}

void Widget::initWidget()
{
  tmpDir = QDir("/tmp/dvtoyinst");

  connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this,
          [&](DGuiApplicationHelper::ColorType themeType)
          {
            if(themeType == DGuiApplicationHelper::DarkType)
            {
                changeTheme(true);
            }
            else
            {
                changeTheme(false);
            }
          }
         );

  unarchiveProcess = new QProcess();

  connect(ui->lblPictureDropArchive, SIGNAL(droppedArchive(QString)), this, SLOT(processVentoyArchive(QString)));
  connect(unarchiveProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [&](int exitCode, QProcess::ExitStatus exitStatus)
        {
          Q_UNUSED(exitCode);
          Q_UNUSED(exitStatus);
          finishedUnarchive();
        });
  connect(ui->btnQueryDeviceVentoyVer, SIGNAL(clicked()), this, SLOT(getDeviceVentoyVersion()));
  connect(ui->btnRefreshDeviceList, SIGNAL(clicked()), this, SLOT(queryDeviceList()));
  connect(ui->btnUsePreviousExtraction, SIGNAL(clicked()), this, SLOT(switchToInstaller()));
  connect(ui->btnInstall, SIGNAL(clicked()), this, SLOT(proceedInstallation()));

  const QString &dviWorkDir = findDVIWorkFile(tmpDir);
  if(dviWorkDir == "")
  {
    ui->btnUsePreviousExtraction->setVisible(false);
  }
  else
  {
    QDir::setCurrent(dviWorkDir);
  }
}

void Widget::on_chkUseCustomVolLabel_stateChanged(int checkState)
{
  if(checkState == Qt::CheckState::Checked)
  {
    ui->lineCustomVolLabel->setEnabled(true);
  }
  else
  {
    ui->lineCustomVolLabel->setEnabled(false);
  }
}

void Widget::on_selectDevice_combo_currentIndexChanged(int index)
{
  Q_UNUSED(index);
  getDeviceVentoyVersion();
}
