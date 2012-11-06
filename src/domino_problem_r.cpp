#include "domino_problem_r.h"


void domino_problem_r::load(QString path)
{
    if(path.toStdString().substr(path.toStdString().find_last_of(".") + 1) == "xml")
       load_xml(path);
    else if (path.toStdString().substr(path.toStdString().find_last_of(".") + 1) == "txt")
       load_txt(path.toStdString());
}

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

void domino_problem_r::load_xml(QString in_path)
{
    size_t x = 0;
    size_t y = 0;
    bool is_vertical = false;
    domino_elem_value_t value1 = 0;
    domino_elem_value_t value2 = 0;

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
            is_vertical = ((e.attribute("orientation","")=="vertical")?true:false);
            value1 = atoi( e.attribute("value1", "").toStdString().c_str() );
            value2 = atoi( e.attribute("value2","").toStdString().c_str());
            x = atoi( e.attribute("x", "").toStdString().c_str() );
            y = atoi( e.attribute("y", "").toStdString().c_str() );
            elem_list.push_back(new domino_elem_located(value1, value2, is_vertical, x, y));
        }
      }
      n = n.nextSibling();
    }
}




void domino_problem_r::load_txt(const std::string &path)
{
   static const int MAX_CHARS_PER_LINE = 128;
   static const int MAX_TOKENS_PER_LINE = 5;
   static const char* const DELIMITER = ";";

   size_t x = 0;
   size_t y = 0;
   bool is_vertical = false;
   domino_elem_value_t value1 = 0;
   domino_elem_value_t value2 = 0;

   // create a file-reading object
   std::ifstream fin;
   fin.open(path); // open a file
   if (!fin.good())
      throw std::runtime_error("Error: *.txt file not found");

   // read each line of the file
   while (!fin.eof())
   {
      // read an entire line into memory
      char buf[MAX_CHARS_PER_LINE];
      fin.getline(buf, MAX_CHARS_PER_LINE);
      // parse the line into blank-delimited tokens
      int n = 0; // a for-loop index
      // array to store memory addresses of the tokens in buf
      const char* token[MAX_TOKENS_PER_LINE] = {0}; // initialize to 0
      // parse the line
      token[0] = strtok(buf, DELIMITER); // first token
      if (token[0]) // zero if line is blank
      {
         for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
         {
            token[n] = strtok(0, DELIMITER); // subsequent tokens
            if (!token[n]) break; // no more tokens
         }
      }

      if (n == 5)
      {
         if (token[0][0] == 'v') is_vertical = true;
         else if (token[0][0] == 'h') is_vertical = false;
         value1 = std::atoi(token[1]);
         value2 = std::atoi(token[2]);
         x = std::atoi(token[3]);
         y = std::atoi(token[4]);
         elem_list.push_back(new domino_elem_located(value1, value2, is_vertical, x, y));
      } else if (n == 2)
      {
         this->board_width = atoi(token[0]);
         this->board_height = atoi(token[1]);
      }
   }
}

