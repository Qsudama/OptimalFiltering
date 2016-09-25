#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QFont>
#include <QFontDatabase>


class FontManager
{

public:
    static FontManager &instance();

    QFont regular(int pointSize = 9) const;
    QFont regularItalic(int pointSize = 9) const;
    QFont regularBold(int pointSize = 9) const;
    QFont regularBoldItalic(int pointSize = 9) const;
    QFont mono(int pointSize = 9) const;
    QFont monoItalic(int pointSize = 9) const;
    QFont monoBold(int pointSize = 9) const;
    QFont monoBoldItalic(int pointSize = 9) const;


private:
    int m_regularId;
    int m_regularItalicId;
    int m_regularBoldId;
    int m_regularBoldItalicId;

    int m_monoId;
    int m_monoItalicId;
    int m_monoBoldId;
    int m_monoBoldItalicId;

    static QFont getFont(int id, int size, bool bold, bool italic);

    FontManager();
    FontManager(const FontManager &) = delete;
    FontManager &operator=(FontManager &) = delete;
};


#endif // FONTMANAGER_H
