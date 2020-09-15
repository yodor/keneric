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

extern "C"
{
    Q_DECL_EXPORT ThumbCreator *new_creator()
    {
        return new Keneric();
    }
}

Keneric::Keneric()
{
}

Keneric::~Keneric()
{
}

bool Keneric::create(const QString& path, int /*width*/, int /*heigth*/, QImage& img)
{
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
    QString program="stripPicture";
    QStringList arguments;
    arguments << path << mime.name() << protoThumbnail;
    QProcess *startAction = new QProcess(parent);
    startAction->start(program, arguments);
    startAction->waitForFinished();
    
    QFile thumbnailFile(protoThumbnail);
    if (thumbnailFile.exists()){
        QImage previewImage(protoThumbnail);
        img = previewImage.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QFile::remove(protoThumbnail);
    }
    
    return !img.isNull();
}

ThumbCreator::Flags Keneric::flags() const
{    
    return (Flags)(None);
}
