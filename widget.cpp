#include <DDialog>
#include <DGuiApplicationHelper>
#include <QProcess>
#include <QStorageInfo>
#include <QMessageBox>

#include "widget.h"
#include "ui_widget.h"
#include "utilities.h"

using namespace Dtk::Gui;
using namespace Dtk::Widget;

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
  m_unarchiveProcess.kill();
  m_unarchiveProcess.waitForFinished();
  if(!tmpDir.removeRecursively())
  {
    qWarning() << "Failed to remove the temporary directory for extraction";
  }
  tmpDir.mkpath(tmpDir.path());
  m_unarchiveProcess.start(QString("tar -xavf %1 -C %2").arg(QUrl(aPath).path()).arg(tmpDir.path()));
}

void Widget::finishedUnarchive()
{
  switch(m_unarchiveProcess.exitCode())
  {
    case 0:
      break;
    case 1:
      qInfo() << "Extracted files differ; they have been updated.";
      break;
    default:
      QMessageBox failExtractMsg;
      failExtractMsg.setText(tr("Extraction of the selected archive was not successful; exit code: %1.")
                             .arg(m_unarchiveProcess.exitCode()));
      failExtractMsg.setIcon(QMessageBox::Critical);
      failExtractMsg.setDetailedText(m_unarchiveProcess.readAllStandardError());
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
  DDialog force, confirm1, confirm2;
  const QString &devicePath = ui->selectDevice_combo->currentData().toString();
  bool isForceInstall, isUpdateInstall;

  isForceInstall = m_DeviceVer >= m_archiveVer;
  isUpdateInstall = m_DeviceVer < m_archiveVer;

  if(isForceInstall)
  {
    force.setMessage(tr("Device %1 has got a newer or same Ventoy installed already. Proceed?\n"
                        "You can still install it forcifully.")
                 .arg(devicePath));
    force.addButton(tr("Confirm"), false, DDialog::ButtonWarning);
    force.addButton(tr("Cancel"), true);
    force.setTitle(tr("Confirm Installation"));
    force.setIcon(QIcon::fromTheme("dialog-warning"));

    if(force.exec() != 0)
    {
      return;
    }
  }

  // We can't reuse a single dialog, because it will mess up
  // the window focuses. Fix this when it behaves right.

  if(!isUpdateInstall)
  {
    confirm1.setMessage(tr("You're about to install Ventoy into %1. Confirm?\n"
                           "ALL your data on the target will be erased!")
                        .arg(devicePath));
    confirm2.setMessage(tr("Double-check! The target device is %1.\n"
                           "Installing Ventoy will destroy ALL DATA present on the device!")
                        .arg(devicePath));
  }
  else
  {
    confirm1.setMessage(tr("You're about to update your Ventoy inside %1. Confirm?\n"
                       "Updating from %2 to %3.")
                        .arg(devicePath)
                        .arg(m_DeviceVer)
                        .arg(m_archiveVer));
  }

  confirm1.addButton(tr("Confirm"), false, DDialog::ButtonWarning);
  confirm1.addButton(tr("Cancel"), true);
  confirm1.setTitle(tr("Confirm Installation"));
  confirm1.setIcon(QIcon::fromTheme("dialog-warning"));

  confirm2.addButton(tr("Confirm"), false, DDialog::ButtonWarning);
  confirm2.addButton(tr("Cancel"), true);
  confirm2.setTitle(tr("Confirm Installation Again"));
  confirm2.setIcon(QIcon::fromTheme("dialog-warning"));

  if(confirm1.exec() == 0)
  {
    if(isUpdateInstall || confirm2.exec() == 0)
    {
      installVentoy(devicePath,
                    m_DeviceVer < m_archiveVer,
                    isForceInstall,
                    ui->chkUseGptPartTable->isChecked(),
                    ui->chkEnableSecureBoot->isChecked(),

                    ui->chkUseCustomVolLabel->isChecked() ?
                      ui->lineCustomVolLabel->text() : "ventoy");
    }
  }
}

void Widget::finishedInstallation()
{
  ui->spinnerInstall->stop();
  ui->spinnerInstall->setVisible(false);
  if(m_installProcess.exitCode())
  {
    ui->lblSuccessFailImage->setPixmap(QIcon::fromTheme("dialog-error").pixmap({48, 48}));
    ui->lblInstallPrompt->setText(tr("Installation failed!"));
  }
  else
  {
    ui->lblSuccessFailImage->setPixmap(QIcon::fromTheme("dialog-ok").pixmap({48, 48}));
    ui->lblInstallPrompt->setText(tr("Installation succeeded!"));
  }
  ui->lblSuccessFailImage->setVisible(true);
}

void Widget::readInstallLog()
{
  ui->textInstallLogs->appendPlainText(m_installProcess.readAll());
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

void Widget::installVentoy(const QString &aDevice, bool aIsUpdate, bool aIsForced,
                           bool aUseGpt, bool aUseSecureBoot, const QString &aVolLabel)
{
  QStringList params;

  if(aIsForced)
  {
    params << "-I";
  }
  else
  {
    if(aIsUpdate)
    {
      params << "-u";
    }
    else
    {
      params << "-i";
    }
  }
  params << aDevice;

  if(aUseGpt)
  {
    params << "-g";
  }
  if(aUseSecureBoot)
  {
    params << "-s";
  }
  if(aVolLabel != "ventoy")
  {
    params << "-L" << aVolLabel;
  }

  m_installProcess.start("Ventoy2Disk.sh", params);
  m_installProcess.write("y\ny\ny\ny\ny\ny\ny\ny\n"); // Gonna perform YES YES YES YES YES...
  ui->mainPager->setCurrentIndex(2);
  ui->spinnerInstall->start();
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

  connect(ui->lblPictureDropArchive, SIGNAL(droppedArchive(QString)), this, SLOT(processVentoyArchive(QString)));
  connect(&m_unarchiveProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [&](int exitCode, QProcess::ExitStatus exitStatus)
        {
          Q_UNUSED(exitCode);
          Q_UNUSED(exitStatus);
          finishedUnarchive();
        });
  connect(&m_installProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [&](int exitCode, QProcess::ExitStatus exitStatus)
        {
          Q_UNUSED(exitCode);
          Q_UNUSED(exitStatus);
          finishedInstallation();
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
  connect(&m_installProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readInstallLog()));

  ui->lblSuccessFailImage->setVisible(false);
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
