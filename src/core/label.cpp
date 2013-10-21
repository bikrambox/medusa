#include "medusa/label.hpp"
#include <algorithm>

MEDUSA_NAMESPACE_BEGIN

Label::Label(std::string const& rName, u16 Type)
  : m_spName(nullptr)
  , m_NameLength(static_cast<u16>(rName.length() + 1))
  , m_Type(Type)
{
  m_spName.reset(new char[m_NameLength], [](char *pName) { delete [] pName; });
  memcpy(m_spName.get(), rName.c_str(), m_NameLength);
}

Label::~Label(void)
{
}

std::string Label::GetLabel(void) const
{
  if (m_NameLength == 0x0)
    return "";

  std::string Result;

  Result.resize(m_NameLength - 1, '_');
  auto pRawName = m_spName.get();
  std::transform(pRawName, pRawName + m_NameLength - 1, Result.begin(), ConvertToLabel);
  return Result;
}

char Label::ConvertToLabel(char c)
{
  /*
  In VC debug, isalnum can assert...
  src: http://msdn.microsoft.com/en-us/library/k84c0490(v=vs.71).aspx
  When used with a debug CRT library, isalnum will display a CRT assert if passed a parameter that isn't EOF or in the range of 0 through 0xFF.
  When used with a debug CRT library, isalnum will use the parameter as an index into an array, with undefined results if the parameter isn't EOF or in the range of 0 through 0xFF.
  */
  int n = c;
  if (n < 0 || n > 0xff)
    return '_';
  if (!isalnum(c) && c != '!' && c != '.')
    return '_';
  return c;
}

MEDUSA_NAMESPACE_END
