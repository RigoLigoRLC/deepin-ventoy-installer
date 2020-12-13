#ifndef UTILITY_H
#define UTILITY_H

#include <QDir>
#include <QCheckBox>

struct SemanticVersion
{
  SemanticVersion();
  SemanticVersion(QString);
  SemanticVersion(uint, uint, uint);
  uint major, minor, patch;
  bool operator==(const SemanticVersion& a) { return (major == a.major) && (minor == a.minor) && (patch == a.patch); }
  bool operator<(const SemanticVersion& a)
  {
    if(major > a.major) return false;
    else if(minor > a.minor) return false;
    else if(patch > a.patch) return false;
    return true;
  }
  bool operator>(const SemanticVersion& a) { return !(*this < a); }
  bool operator<=(const SemanticVersion& a) { return ((*this < a) || (*this == a));}
  bool operator>=(const SemanticVersion& a) { return (!(*this < a)) || (*this == a); }
  operator QString() { return QString("%1.%2.%3").arg(major).arg(minor).arg(patch); }

};

QString findDVIWorkFile(const QDir &dviTmpDir);
void rerunOnNonRoot();
void enableCheckBox(bool criteria, QCheckBox* w);

#endif // UTILITY_H
