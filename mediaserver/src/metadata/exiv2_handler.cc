/*  metadata_reader.cc - this file is part of MediaTomb.

    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
    Sergey Bostandzhyan <jin@deadlock.dhs.org>

    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_EXIV2

/// \file exiv2_handler.cc
/// \brief Implementeation of the Exiv2Handler class.

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

#include "exiv2_handler.h"
#include "string_converter.h"

using namespace zmm;

Exiv2Handler::Exiv2Handler() : MetadataHandler()
{
}
       
void Exiv2Handler::fillMetadata(Ref<CdsItem> item)
{
    Ref<StringConverter> sc = StringConverter::m2i();

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(std::string(item->getLocation().c_str()));
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    // first retrieve jpeg comment
    String comment = (char *)image->comment().c_str();

    if (exifData.empty())
    {
        // no exiv2 record found in image
        return;
    }
  
    // get date/time
    Exiv2::ExifData::const_iterator md = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
    if (md != exifData.end()) 
    {
        // \TODO convert date to ISO 8601 as required in the UPnP spec
        item->setMetadata(String(MT_KEYS[M_DATE].upnp), sc->convert(String((char *)md->toString().c_str())));
    }

    // if there was no jpeg coment, look if there is an exiv2 comment
    // should we override the normal jpeg comment, if there is an exiv2 one?
    if (!string_ok(comment))
    {
        md = exifData.findKey(Exiv2::ExifKey("Exif.Photo.UserComment"));
        if (md != exifData.end())
            comment = (char *)md->toString().c_str();
    }
    
    // if the image has no comment, compose something nice out of the exiv information
    if (!string_ok(comment))
    {
        String cam_model;
        String flash;
        String focal_length;

        md = exifData.findKey(Exiv2::ExifKey("Exif.Image.Model"));
        if (md !=  exifData.end())
            cam_model = (char *)md->toString().c_str();

        md = exifData.findKey(Exiv2::ExifKey("Exif.Photo.Flash"));
        if (md !=  exifData.end())
            flash = (char *)md->toString().c_str();

        md = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength"));
        if (md !=  exifData.end())
        {
            focal_length = (char *)md->toString().c_str();
            md = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLengthIn35mmFilm"));
            if (md !=  exifData.end())
            {
                focal_length = focal_length + " (35 mm equivalent: " + (char *)md->toString().c_str() + ")";
            }
        }


        if (string_ok(cam_model))
            comment = String("Taken with ") + cam_model;

        if (string_ok(flash))
        {
            if (string_ok(comment))
                comment = comment + ", Flash setting:" + flash;
            else
                comment = String("Flash setting: ") + flash;
        }

        if (string_ok(focal_length))
        {
            if (string_ok(comment))
                comment = comment + ", Focal length: " + focal_length;
            else
                comment = String("Focal length: ") + focal_length;
        }
    }

    if (string_ok(comment))
        item->setMetadata(String(MT_KEYS[M_DESCRIPTION].upnp), sc->convert(comment));

}

Ref<IOHandler> Exiv2Handler::serveContent(Ref<CdsItem> item, int resNum)
{
    return nil;
}

#endif // HAVE_EXIV2
