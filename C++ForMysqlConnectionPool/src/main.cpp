#include <iostream>
#include "ConnectionPool.h"
#include "MysqlConn.h"
void query() {

   MysqlConn conn;
   int ref= conn.connect("root", "mysql", "dev_spims", "127.0.0.1", 3306);
   if (ref!=0)
   {
       std::cout << "Hello World!\n";


   }

}
int main()
{
    query();
   /* ConnectionPool* pool = ConnectionPool::getConnectionPool();
    std::shared_ptr<MysqlConn*> conn = pool->getConnection();*/
    return 0;
    
        
}



