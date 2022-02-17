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
//#include <iostream>

extern "C"
{
    Q_DECL_EXPORT ThumbCreator *new_creator()
    {
        return new Keneric();
    }
}

Keneric::Keneric()
{
    //std::cout << "Keneric CTOR" << std::endl;
}

Keneric::~Keneric()
{
    //std::cout << "Keneric DTOR" << std::endl;
}

bool Keneric::create(const QString& path, int width, int height, QImage& img)
{
    
    //std::cout << "Create Width: " << width << std::endl;
    //std::cout << "Create Height: " << width << std::endl;
    
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(path);

    QString kenericDirectory((QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) + "/keneric/"));
    QString md5Hash = QString(QCryptographicHash::hash((path.toUtf8()),QCryptographicHash::Md5).toHex());
    QString protoThumbnail(kenericDirectory + md5Hash);
    
    QDir directory(kenericDirectory);
    if (!directory.exists()) {
        directory.mkpath(".");
    }

    QObject *parent = 0;
    QString program="kenericProcess";
    QStringList arguments;
    arguments << path << mime.name() << protoThumbnail << QString("%1").arg(width) << QString("%1").arg(height);
    
    QProcess *startAction = new QProcess(parent);
    startAction->start(program, arguments);
    startAction->waitForFinished();
    
    QFile thumbnailFile(protoThumbnail);
    if (thumbnailFile.exists()){
        QImage previewImage(protoThumbnail);
        previewImage = previewImage.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        img.swap(previewImage);
        QFile::remove(protoThumbnail);
    }
    
    return !img.isNull();
}

ThumbCreator::Flags Keneric::flags() const
{    
    return (Flags)(None);
}
