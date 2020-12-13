#ifndef WIDGET_H
#define WIDGET_H

#include "utilities.h"

#include <QWidget>
#include <QThread>
#include <QProcess>
#include <QDir>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

  public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

  public slots:
    void processVentoyArchive(QString aPath);

    /**
     * @brief get the Ventoy version in selected device. Must select a device.
     */
    void getDeviceVentoyVersion();

    /**
     * @brief query the available storage devices and refresh combo box.
     */
    void queryDeviceList();

    /**
     * @brief switch pager to install details page.
     */
    void switchToInstaller();

    /**
     * @brief set the enabled property of UI according to m_archiveVer
     */
    void verifyVentoyFeatures();

  private slots:
    void finishedUnarchive();

    void proceedInstallation();

  private slots:
    void on_chkUseCustomVolLabel_stateChanged(int arg1);

    void on_selectDevice_combo_currentIndexChanged(int index);

  private:
    /**
     * @brief initialize main widget.
     */
    void initWidget();

    /**
     * @brief change dark/light theme, mostly for button icons.
     * @param aDark true for dark theme, false for light theme.
     */
    void changeTheme(bool aDark);

    /**
     * @brief get the Ventoy version in extracted archive. Must have archive extracted.
     */
    void getArchiveVentoyVersion();


  private:
    Ui::Widget *ui;
    QDir tmpDir, ventoyDir;
    QProcess *unarchiveProcess;
    SemanticVersion m_archiveVer, m_DeviceVer;
};

#endif // WIDGET_H
