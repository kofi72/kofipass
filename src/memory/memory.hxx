#ifndef MEMORY_HXX_INCLUDED
#define MEMORY_HXX_INCLUDED

#include <string>

class safe_string : public std::string {
public:
    safe_string();
    safe_string(const char* s);
    safe_string(const std::string& s);
    safe_string(const safe_string& s);
    safe_string(const std::string&& s);

    ~safe_string();
    safe_string& operator=(const safe_string& s);
    bool operator==(const safe_string& s) const;
    void clear() noexcept;

private:
    void secure_erase();
};

#endif // MEMORY_HXX_INCLUDED
