/* File: sqlite_thread.cpp
   Purpose: ohjelmat.db tietokannan hallinta

*/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <SDL.h>

#include "var.h"
#include "kettle_functions.h"
#include "externs.h"
#include "cook_prog.h"

extern int Stop_Threads;
int sqlite_uusi_ohjelma_tehty, sqlite_busy, sqlite_uusi_haku_halutaan, sql_haku_idx;

sql_fields_s sql_haku[SQLITE_HAKU_MAX];

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) 
   {
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      if ( strcmp( azColName[i], "nimi") == 0)
      {
         if (sql_haku_idx < SQLITE_HAKU_MAX)
            strcpy( sql_haku[sql_haku_idx].haku1, argv[i]);
      }
      else if ( strcmp( azColName[i], "erakoko") == 0)
      {
         if (sql_haku_idx < SQLITE_HAKU_MAX)
            strcpy( sql_haku[sql_haku_idx].haku2, argv[i]);
      }
      else if ( strcmp( azColName[i], "vaihe_lkm") == 0)
      {
         if (sql_haku_idx < SQLITE_HAKU_MAX)
            strcpy( sql_haku[sql_haku_idx].haku3, argv[i]);
      }
   }
//   printf("\n");
   sql_haku_idx++;
   return 0;
}

char sql_buf[500];
char sql_kansio[30];

int sqlite_thread(void* data )
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;

   printf("sqlite thread started\n");

   /* Open database */
   rc = sqlite3_open("testdb/ohjelmat.db", &db);
   
   if (rc) 
   {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      while (Stop_Threads == 0)
         SDL_Delay(500);
      return(0);
   }
   else 
   {
      fprintf(stderr, "Opened database testdb.ohjelmat successfully\n");
   }
   sqlite_uusi_ohjelma_tehty = 0;
   sqlite_busy = 0;
   sqlite_uusi_haku_halutaan = 0;

   while (Stop_Threads == 0)
   {
      SDL_Delay(200);
      if (sqlite_uusi_ohjelma_tehty)
      {
         sqlite_busy = 1;
         sqlite_uusi_ohjelma_tehty = 0;
         sprintf(sql_buf, "INSERT INTO ohjelmat (kansio, nimi, oma_kansio, vaihe_lkm, tyokalu, erakoko) VALUES ('%s', '%s', %d, %d, %d, %d)", 
                        tmp_kansion_nimi, tmp_ohj_nimi, tmp_header.Oma_kansio, tmp_header.Vaihe_lkm, tmp_header.Tyokalu_idx, tmp_header.Erakoko_kg);
         sql = sql_buf;

         /* Execute SQL statement */
         rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            
         if( rc != SQLITE_OK )
         {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            break;
         } 
         else 
            fprintf(stdout, "Records created successfully\n");
         sqlite_busy = 0;
      }
      else if (sqlite_uusi_haku_halutaan)
      {
         sqlite_busy = 1;
         if (sqlite_uusi_haku_halutaan == 1) //Aakkosjärj
         {
            sprintf(sql_buf, "select nimi,erakoko,vaihe_lkm from ohjelmat where kansio = '%s' order by nimi;", sql_kansio);
         }
         else if (sqlite_uusi_haku_halutaan == 2) //Lisäysjärj
         {
            sprintf(sql_buf, "select nimi,erakoko,vaihe_lkm from ohjelmat where kansio = '%s';", sql_kansio);
         }
         sql = sql_buf;
         sqlite_uusi_haku_halutaan = 0;
         sql_haku_idx = 0;
         /* Execute SQL statement */
         rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            
         if( rc != SQLITE_OK )
         {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            break;
         } 
         sqlite_busy = 0;

         //for (int i=0; i < sql_haku_idx; i++)
         //{
         //   printf("Hakutulos %d=%s\n", i, sql_haku[i].haku1);
         //}
      }
   }	

   sqlite3_close(db);
   fprintf(stderr, "Database closed\n");
	return 0;
}

/*
   sql = "CREATE TABLE ohjelmat (" \
         "ohjelma_id INTEGER PRIMARY KEY," \
         "kansio TEXT," \
         "nimi TEXT," \
         "oma_kansio INTEGER," \
         "vaihe_lkm INTEGER," \
         "tyokalu INTEGER," \
         "erakoko INTEGER," \
         "kuvaus TEXT," \
         "suosikki INTEGER," \
         "ajokerrat INTEGER); ";
*/

// Uuden ohjelman 1. riville on kirjoitettu
// sprintf(buffer, "%s,%s,%d,%d,%d,%d,", tmp_kansion_nimi, tmp_ohj_nimi, tmp_header.Oma_kansio, tmp_header.Vaihe_lkm, tmp_header.Tyokalu_idx, tmp_header.Erakoko_kg);

int sqlite_insert_ohjelma(void)
{
   int timeout=0;

   while(sqlite_busy)
   {
      SDL_Delay(500);
      if (++timeout > 3)
      {
         printf("Sqlite busy timeout!\n");
         return -1;
      }
   }
   sqlite_uusi_ohjelma_tehty = 1;
   sqlite_busy = 1;
   return 0;
}

int sqlite_hae_ohjelmia(char *kansiosta)
{
   int timeout=0;

   while(sqlite_busy)
   {
      SDL_Delay(500);
      if (++timeout > 3)
      {
         printf("Sqlite busy timeout!\n");
         return -1;
      }
   }
   sql_haku_idx = 0;
   sqlite_busy = 1;
   strcpy(sql_kansio, kansiosta);
   sqlite_uusi_haku_halutaan = 1;
   return 0;
}
