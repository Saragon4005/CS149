/**
 * Description  name-count hash table and the
 *              per-file thread worker used by both shell and countnames.
 *              The hash table grows dynamically and is thread-safe.  every
 *              call into insertName has an internal mutex.
 *              printTable() and freeTable() are main thread only
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 5/4/26
 * Creation date: 5/1/26
 **/

#ifndef NAMECOUNT_H
#define NAMECOUNT_H

 // not thread safe
void printTable();
void freeTable();
void initTable();


void *countNamesThread(void *); // thread safe

#endif /* NAMECOUNT_H */
