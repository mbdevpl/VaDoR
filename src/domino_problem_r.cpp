#include "domino_problem_r.h"

QDomDocument domino_problem_r::open_file(QString in_path)
{
    QDomDocument doc( "xml_board" );
    QFile file( in_path );
    if( !file.open( QIODevice::ReadOnly ) )
        throw "Erorr occured while opening the file";
    if( !doc.setContent( &file ) )
    {
      file.close();
      throw "Error occured while setting content of a xml document";
    }
    file.close();

    return doc;
}

void domino_problem_r::load(QString in_path)
{
    QDomDocument doc = open_file(in_path);
    QDomElement root = doc.documentElement();
    if( root.tagName() != "domino_board" )
        throw "Error: bad input file";

    this->board_width = atoi( root.attribute("width","").toStdString().c_str() );
    this->board_height = atoi( root.attribute("height","").toStdString().c_str() );

    QDomNode n = root.firstChild();
    while( !n.isNull() )
    {
      QDomElement e = n.toElement();
      if( !e.isNull() )
      {
        if( e.tagName() == "piece" )
        {
            domino_elem *piece = new domino_elem(atoi( e.attribute("value1", "").toStdString().c_str() ),
               atoi( e.attribute("value2","").toStdString().c_str()),((e.attribute("orientation","")=="vertical")?true:false)
                    );
          piece->h1.loc_x = atoi( e.attribute("x", "").toStdString().c_str() );
          piece->h1.loc_y = atoi( e.attribute("y", "").toStdString().c_str() );
          piece->h2.loc_x = piece->h1.loc_x + (piece->is_vertical ? 0 : 1);
          piece->h2.loc_y = piece->h1.loc_y + (piece->is_vertical ? 1 : 0);

          elem_list.push_back(piece);
        }
      }
      n = n.nextSibling();
    }
}

