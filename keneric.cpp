//    Copyright (C) 2015 Rog131 <samrog131@hotmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "keneric.h"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QProcess>
#include <QMimeType>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <iostream>

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(Keneric, "keneric.json")

Keneric::Keneric(QObject *parent, const QVariantList &args)
    : KIO::ThumbnailCreator(parent, args)
{
    std::cout << "Keneric CTOR" << std::endl;
}

Keneric::~Keneric()
{
    std::cout << "Keneric DTOR" << std::endl;
}

KIO::ThumbnailResult Keneric::create(const KIO::ThumbnailRequest &request)
{
    

    
    QString path = request.url().toLocalFile();
    std::string path_string = path.toStdString();
    //const char* path_cstring = path_string.c_str();
    int width  = request.targetSize().width() * request.devicePixelRatio();
    int height = request.targetSize().height() * request.devicePixelRatio();

    //std::cout << "Create Width: " << width << std::endl;
    //std::cout << "Create Height: " << height << std::endl;


    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(path);

    QString kenericDirectory((QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) + QString::fromLocal8Bit("/keneric/")));

    QByteArray md5Hash = QCryptographicHash::hash(path.toUtf8(),QCryptographicHash::Md5).toHex();

    QString protoThumbnail(kenericDirectory);
    protoThumbnail.append(QString::fromLocal8Bit(md5Hash));

    QDir directory(kenericDirectory);
    if (!directory.exists()) {
        directory.mkpath(QString::fromLocal8Bit("."));
    }

    QString program = QString::fromLocal8Bit("kenericProcess");

    QStringList arguments;
    arguments << path;
    arguments << mime.name();
    arguments << protoThumbnail;
    arguments << QString(QString::fromLocal8Bit("%1")).arg(width);
    arguments << QString(QString::fromLocal8Bit("%1")).arg(height);

    QProcess *process = new QProcess();
    process->start(program, arguments);
    process->waitForFinished();

    QFile thumbnailFile(protoThumbnail);
    if (thumbnailFile.exists()){
        QImage preview(protoThumbnail);
        preview = preview.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QFile::remove(protoThumbnail);
        return KIO::ThumbnailResult::pass(preview);
    }
    else {
        std::cout << "File not found: " << protoThumbnail.toStdString() << std::endl;
    }
    return KIO::ThumbnailResult::fail();


}



#include "keneric.moc"
#include "moc_keneric.cpp"


