<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.cpp" line="27"/>
        <source>There is still ongoing tasks running!
Terminating the program right now could cause problems.

Continue?</source>
        <translation>有未完成的任务正在进行！
现在终止程序可能导致问题。

继续吗？</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="30"/>
        <source>Exit</source>
        <translation>退出程序</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="32"/>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="32"/>
        <source>No</source>
        <translation>否</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="52"/>
        <source>Go back to drop archive</source>
        <translation>返回拖放压缩包</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="main.cpp" line="19"/>
        <source>Deepin Ventoy Installer</source>
        <translation>Deepin Ventoy 安装器</translation>
    </message>
    <message>
        <location filename="main.cpp" line="20"/>
        <source>A Ventoy installer front-end written with DTK.</source>
        <translation>一个Ventoy的DTK版安装器前端</translation>
    </message>
    <message>
        <location filename="utilities.cpp" line="93"/>
        <source>Run As Root</source>
        <translation>需以root用户运行</translation>
    </message>
    <message>
        <location filename="utilities.cpp" line="94"/>
        <source>This application must be run as root user.
Provide password to continue.
</source>
        <translation>本程序必须以root用户运行。
请提供密码以继续。
</translation>
    </message>
    <message>
        <location filename="utilities.cpp" line="97"/>
        <source>Authenticate</source>
        <translation>验证</translation>
    </message>
</context>
<context>
    <name>Widget</name>
    <message>
        <location filename="widget.ui" line="26"/>
        <source>Form</source>
        <translation></translation>
    </message>
    <message>
        <location filename="widget.ui" line="55"/>
        <source>Drag and drop your Ventoy archive
named &quot;ventoy-x.y.z-linux.tar.gz&quot; here</source>
        <translation>把“ventoy-x.y.z.tar.gz”
名称的压缩包拖放至此</translation>
    </message>
    <message>
        <location filename="widget.ui" line="66"/>
        <source>Use the previous extraction in temporary folder</source>
        <translation>使用缓存中已解压的文件</translation>
    </message>
    <message>
        <location filename="widget.ui" line="131"/>
        <source>Selected Ventoy
Archive Version</source>
        <translation>已选择的
Ventoy版本</translation>
    </message>
    <message>
        <location filename="widget.ui" line="196"/>
        <source>Ventoy Version
in Device</source>
        <translation>设备内的
Ventoy版本</translation>
    </message>
    <message>
        <location filename="widget.ui" line="268"/>
        <source>Additional Configuration</source>
        <translation>附加设置</translation>
    </message>
    <message>
        <location filename="widget.ui" line="277"/>
        <source>Enable Secure Boot support (experimental)</source>
        <translation>启用安全启动支持（实验性）</translation>
    </message>
    <message>
        <location filename="widget.ui" line="284"/>
        <source>Use GPT partition table for target device</source>
        <translation>目标设备使用GPT分区表</translation>
    </message>
    <message>
        <location filename="widget.ui" line="293"/>
        <source>Custom volume label</source>
        <translation>自定义卷标</translation>
    </message>
    <message>
        <location filename="widget.ui" line="306"/>
        <source>ventoy</source>
        <translation>ventoy</translation>
    </message>
    <message>
        <location filename="widget.ui" line="318"/>
        <source>Install</source>
        <translation>安装</translation>
    </message>
    <message>
        <location filename="widget.ui" line="377"/>
        <source>Installing in progress, please wait...</source>
        <translation>正在安装，请稍候……</translation>
    </message>
    <message>
        <location filename="widget.ui" line="396"/>
        <source>Go back</source>
        <translation>返回</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="57"/>
        <source>Extraction of the selected archive was not successful; exit code: %1.</source>
        <translation>选择的压缩包解压失败。返回值为%1。</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="73"/>
        <source>No working Ventoy build can be found in the extracted archive.</source>
        <translation>解压后的Ventoy文件无法识别。</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="95"/>
        <source>Device %1 has got a newer or same Ventoy installed already. Proceed?
You can still install it forcifully.</source>
        <translation>%1设备中的Ventoy版本与当前版本相同或更新。
确认吗？你可以强行安装。</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="98"/>
        <location filename="widget.cpp" line="130"/>
        <location filename="widget.cpp" line="135"/>
        <source>Confirm</source>
        <translation>确认</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="99"/>
        <location filename="widget.cpp" line="131"/>
        <location filename="widget.cpp" line="136"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="100"/>
        <location filename="widget.cpp" line="132"/>
        <source>Confirm Installation</source>
        <translation>确认安装</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="114"/>
        <source>You&apos;re about to install Ventoy into %1. Confirm?
ALL your data on the target will be erased!</source>
        <translation>你将要在设备%1上安装Ventoy。确认吗？
目标设备上的所有数据都会被擦除！</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="117"/>
        <source>Double-check! The target device is %1.
Installing Ventoy will destroy ALL DATA present on the device!</source>
        <translation>请再次确认！目标设备是%1。
在它上面安装Ventoy会擦除内部的所有数据！</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="123"/>
        <source>You&apos;re about to update your Ventoy inside %1. Confirm?
Updating from %2 to %3.</source>
        <translation>将对%1上的Ventoy进行更新。确认吗？
从%2更新到%3。</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="137"/>
        <source>Confirm Installation Again</source>
        <translation>再次确认安装</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="164"/>
        <source>Installation failed!</source>
        <translation>安装失败！</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="169"/>
        <source>Installation succeeded!</source>
        <translation>安装成功！</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="221"/>
        <source>Cannot query available devices; exit code: %1.</source>
        <translation>无法获取当前设备。返回值为%1。</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="353"/>
        <source>Will force install to the drive!</source>
        <translation>将强行安装到设备上！</translation>
    </message>
    <message>
        <location filename="widget.cpp" line="357"/>
        <source>Will upgrade Ventoy. Volume label will remain untouched.</source>
        <translation>将更新设备内Ventoy版本。卷标不会改变。</translation>
    </message>
</context>
</TS>
