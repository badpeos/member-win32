#ifndef MAIN_H
#define MAIN_H

#endif

#include <string>
#include <deque>

class Database;

Database &getDatabase();

#ifdef __cplusplus
extern "C" {
#endif

HINSTANCE getInstance(void);
int initListView(HWND hWnd, UINT ctrlID, std::deque<std::string> &header);

#ifdef __cplusplus
}
#endif
