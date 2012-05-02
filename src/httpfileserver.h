/*  This file is part of the Tufão project
    Copyright (C) 2012 Vinícius dos Santos Oliveira <vini.ipsmaker@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any
    later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUFAO_HTTPFILESERVER_H
#define TUFAO_HTTPFILESERVER_H

#include "abstracthttpserverrequesthandler.h"
#include "headers.h"
#include "httpserverresponse.h"

class QFileInfo;

namespace Tufao {

/*!
  You can use this class to serve static files under Tufão. It provides a robust
  HTTP file server, supporting conditional and byte-range requests.

  The two main approaches are:
    - Construct an object and use the AbstractHttpServerRequestHandler API
      implemented by HttpFileServer
    - Use the static methods to serve a file (or set the entity in the response
      body)

  The algorithm used to serve files will handle the following set of headers:
    - If-Modified-Since
    - If-Unmodified-Since
    - If-Range
    - Range

  It won't handle:
    - ETag (If-Match and If-None-Match headers)
    - Cache-Control response header: Useful for set cache max age
    - Content-Disposition response header
    - Content-MD5 response header
    - Content-Type response header. This header is used to inform the mime type
      of the file to the client.

  \since
  0.3
  */
class TUFAO_EXPORT HttpFileServer: public AbstractHttpServerRequestHandler
{
    Q_OBJECT
public:
    /*!
      Constructs a HttpFileServer object.

      \p parent is passed to the QObject constructor.

      \p dir is used as root dir to serve files.
      */
    explicit HttpFileServer(const QString &dir, QObject *parent = 0);

    /*!
      Destroys the object.
      */
    ~HttpFileServer();

    /*!
      Set the root dir.

      The root dir is the dir containing the files to be served by the
      HttpFileServer object.
      */
    void setDir(const QString &dir);

    /*!
      Return the root dir containing the files to be served.
      */
    QString dir() const;

    /*!
      Analyze the \p request and serve the file pointed by \p filename.
      */
    static void serveFile(const QString &fileName, HttpServerRequest *request,
                          HttpServerResponse *response);

    /*!
      This member function doesn't serve any file, just set the response body to
      the contents in the file pointed by \p filename. It's useful in some
      scenarios, like serving 404-pages.
      */
    static bool serveFile(const QString &fileName, HttpServerResponse *response,
                          int statusCode);

    /*!
      Return the buffer size used.

      When serving files, HttpFileServer allocates some bytes of the file in the
      memory before sending it to the network. The maximum space allocated is
      the buffer size. This method returns what number is this.

      \note
      The buffer size is global to all HttpFileServer objects.

      \sa
      setBufferSize
      */
    static qint64 bufferSize();

    /*!
      Set the buffer size.
      */
    static void setBufferSize(qint64 size);

public slots:
    /*!
      It searchs for the file requested in the root dir and respond to the
      request, if the file is found.

      \note
      This method won't let requests access files outside the root dir folder
      and should be prefered over self-made implementations, as its safer.
      */
    bool handleRequest(Tufao::HttpServerRequest *request,
                       Tufao::HttpServerResponse *response,
                       const QStringList & = QStringList());

private:
    struct Priv;
    Priv *priv;
};

} // namespace Tufao

#endif // TUFAO_HTTPFILESERVER_H