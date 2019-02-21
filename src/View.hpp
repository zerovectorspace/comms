#ifndef MESSY_WINDOW_HPP
#define MESSY_WINDOW_HPP

#include <ncurses.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <deque>

template <typename T>
using Vec = std::vector<T>;

using String = std::vector<char>;
using StdString = std::string;

class WinBuf
{
public:
    WINDOW* window = nullptr;

    std::deque<Vec<char>> buffer{};

    bool is_writable = true;
    int w{}, h{}, x{}, y{};
};

class View
{
    std::unordered_map<StdString,std::unique_ptr<WinBuf>> window_map{};
    int w{}, h{};

public:
    StdString current_win{};

    ~View();

    void get_win_dims();

    void init();

    Vec<StdString> win_list();

    void new_win(const StdString& name, const int& w, const int& h, const int& x, const int& y, bool is_writable);

    void del_win(const StdString& win);

    void refresh(const StdString& win);

    void refresh();

    void append(const StdString& win, const char* buf);

    void appendln(const StdString& win, const char* buf);

    void clear_buffer(const StdString& win);

    void clear_win(const StdString& win);

    void fresh_print(const StdString& win, const char* buf);

    void fresh_print(const char* buf);

    void print(const StdString& win, const char* buf);

    void fresh_flush(const StdString& win);

    void flush(const StdString& win);

    void move(const StdString& win, const int& x, const int& y);

    char get_char(const StdString& win);

    Vec<char> get_line(const StdString& win, bool with_zero = true);

    Vec<char> get_multi_line(const StdString& win);

    WinBuf* get_window(const StdString& win);

    int width();

    int height();

    bool is_writable( StdString const& win );

    bool is_writable();
};

#endif
