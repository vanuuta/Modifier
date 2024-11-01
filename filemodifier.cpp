#include "filemodifier.h"


FileModifier::FileModifier(const QString &inputMask, bool deleteAfterProcessing, const QString &inputDir, const QString &outputDir, const QString &overwriteMode, int pollingInterval, const QByteArray &xorValue):
    inputMask(inputMask), deleteAfterProcessing(deleteAfterProcessing),inputDir(inputDir),
    outputDir(outputDir), overwriteMode(overwriteMode),
    pollingInterval(pollingInterval), xorValue(xorValue)
{
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &FileModifier::checkFiles);
    timer->start(pollingInterval * 1000); // интервал в секундах
}

void FileModifier::checkFiles() {
    //QDir dir(QDir::currentPath()+"/input_files");
    QDir dir(inputDir);
    //qDebug() << "Путь к директории:" << dir.absolutePath();
    QStringList filters;
    filters << inputMask; // Добавляем маску в список фильтров
    QStringList files = dir.entryList(filters, QDir::Files);
    //qDebug() << files;
    for (const QString &fileName : files) {
        processFile(fileName);
    }
}

void FileModifier::processFile(const QString &fileName) {
    //QDir dir(QDir::currentPath());
    //QDir dir(inputDir);
    //QString absPath = dir.absolutePath();
    //qDebug() << "ABS" << absPath ;
    //absPath+="/input_files/";
    QFile file(inputDir+"/" + fileName);
    //()<< "absPath + fileName = " << absPath + fileName;
    QFileInfo fileInfo(inputDir+"/" + fileName);
    QDateTime lastModified = fileInfo.lastModified();
    QDateTime currentTime = QDateTime::currentDateTime();
    if (lastModified.secsTo(currentTime) < 10) {
        file.close();
        return ;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Файл занят другим процессом или не существует:";
        return;
    }

    QByteArray fileData = file.readAll();
    QByteArray modifiedData = xorData(fileData);
    QString outputFileName = generateOutputFileName(fileName);
    QFile outputFile(outputFileName);
    if (outputFile.open(QIODevice::WriteOnly)) {
        outputFile.write(modifiedData);
        outputFile.close();
    }
    if (deleteAfterProcessing) {
        file.remove();
    }
    file.close();
}
QByteArray FileModifier::xorData(const QByteArray &data) {
    QByteArray result = data;
    for (int i = 0; i < result.size(); ++i) {
        result[i] = result[i] ^ xorValue[i % xorValue.size()];
    }
    return result;
}

QString FileModifier::generateOutputFileName(const QString &inputFileName) {
    QFileInfo fileInfo(inputFileName);
    QString baseName = fileInfo.baseName();
    QString extension = fileInfo.suffix();
    QString outputFileName = QDir(outputDir).filePath(baseName + "." + extension);

    if (overwriteMode == "Перезаписать") {
        return outputFileName;
    } else {
        int counter = 1;
        while (QFile::exists(outputFileName)) {
            outputFileName = QDir(outputDir).filePath(baseName + "_" + QString::number(counter++) + "." + extension);
        }
        return outputFileName;
    }
}





//------------------------------------------------------------------------
// bool FileModifier::isFileOpen(const QString& filePath) {
//     HANDLE fileHandle = CreateFile(
//         reinterpret_cast<LPCWSTR>(filePath.unicode()), // Преобразуем QString в LPCWSTR
//         GENERIC_READ,
//         0,  // No sharing
//         NULL,
//         OPEN_EXISTING,
//         FILE_ATTRIBUTE_NORMAL,
//         NULL);
//     qDebug()<< "filePath = " <<filePath;
//     if (fileHandle == INVALID_HANDLE_VALUE) {
//         // Файл занят или не существует
//         return true;
//     } else {
//         // Файл успешно открыт
//         CloseHandle(fileHandle);
//         return false;
//     }
// }




