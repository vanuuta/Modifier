#ifndef FILEMODIFIER_H
#define FILEMODIFIER_H
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QtCore>
#include <QMessageBox>
#include <windows.h>
#include <fileapi.h>
#include <QFileInfo>
class FileModifier:public QObject
{
private:
    QString inputMask;
    bool deleteAfterProcessing;
    QString outputDir;
    QString overwriteMode;
    int pollingInterval;
    QByteArray xorValue;
    //unsigned char key;
    QTimer *timer;
public:
    explicit FileModifier(const QString &inputMask, bool deleteAfterProcessing, const QString &outputDir,
                          const QString &overwriteMode, int pollingInterval, const QByteArray &xorValue);
    bool isFileOpen(const QString& filePath);
    void processFile(const QString &fileName);
    QByteArray xorData(const QByteArray &data);
    QString generateOutputFileName(const QString &inputFileName);
public slots:
    void checkFiles();

};

#endif // FILEMODIFIER_H
