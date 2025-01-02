#include "athena/WiiFile.hpp"
#include "athena/Utility.hpp"
#include <algorithm>

namespace athena {

//! TODO: Remove this?
WiiFile::WiiFile()
: m_permissions(WiiFile::GroupRW | WiiFile::OtherRW | WiiFile::OwnerRW)
, m_attributes(0)
, m_type(WiiFile::File)
, m_filename("")
, m_fileLen(0)
, m_fileData(NULL)
, m_parent(NULL) {
  // ctor
}

WiiFile::WiiFile(const std::string& filename)
: m_permissions(WiiFile::GroupRW | WiiFile::OtherRW | WiiFile::OwnerRW)
, m_attributes(0)
, m_type(WiiFile::File)
, m_filename(filename)
, m_fileLen(0)
, m_fileData(NULL)
, m_parent(NULL) {}

WiiFile::WiiFile(const std::string& filename, atUint8 permissions, const atUint8* data, atUint32 length)
: m_permissions(permissions)
, m_attributes(0)
, m_type(WiiFile::File)
, m_filename(filename)
, m_fileLen(length)
, m_fileData((atUint8*)data) {}

WiiFile::~WiiFile() {
  if (m_fileData)
    delete[] m_fileData;

  for (WiiFile* child : m_children)
    delete child;
}

void WiiFile::setFilename(const std::string& filename) { m_filename = filename; }

std::string WiiFile::filename() const { return m_filename; }

void WiiFile::setPermissions(const atUint8 permissions) { m_permissions = (atUint8)permissions; }

atUint8 WiiFile::permissions() const { return m_permissions; }

void WiiFile::setData(const atUint8* data) {
  if (m_fileData) {
    delete[] m_fileData;
    m_fileData = NULL;
  }

  m_fileData = (atUint8*)data;
}

atUint8* WiiFile::data() const { return m_fileData; }

void WiiFile::setLength(const int len) { m_fileLen = (int)len; }

int WiiFile::length() const { return m_fileLen; }

void WiiFile::setAttributes(const atUint8 attr) { m_attributes = attr; }

atUint8 WiiFile::attributes() const { return m_attributes; }

void WiiFile::setType(WiiFile::Type type) { m_type = type; }

WiiFile::Type WiiFile::type() const { return m_type; }

bool WiiFile::isDirectory() const { return (m_type == WiiFile::Directory); }

bool WiiFile::isFile() const { return (m_type == WiiFile::File); }

void WiiFile::addChild(WiiFile* file) {
  if (!isDirectory()) {
    atWarning("{} is not a directory", m_filename);
    return;
  }

  if (std::find(m_children.begin(), m_children.end(), file) != m_children.end())
    return;

  // Lets figure out it's place
  std::string tmpName(file->filename());
  // Since we only support *NIX paths this is simple
  atUint32 depth = athena::utility::countChar(tmpName, '/');
  bool owned = false;

  while ((depth--) > 0) {
    // add them from the beginning of the path up
    tmpName = tmpName.substr(0, tmpName.find('/'));

    for (atUint32 i = 0; i < m_children.size(); i++) {
      if (m_children[i]->filename() == tmpName) {
        std::string newName = file->filename();
        newName = newName.substr(newName.rfind("/") + 1, newName.size() - newName.rfind("/"));
        file->setFilename(newName);
        m_children[i]->addChild(file);
        owned = true;
      }
    }
  }

  if (!owned) {
    m_children.push_back(file);
    file->setParent(this);
  }
}

WiiFile* WiiFile::child(const std::string& name) {
  std::vector<WiiFile*>::iterator iter =
      std::find_if(m_children.begin(), m_children.end(), [&name](WiiFile* f) { return f->filename() == name; });

  if (iter != m_children.end())
    return *iter;

  std::string tmpName(name);
  tmpName = tmpName.substr(tmpName.rfind('/') + 1, tmpName.size() - tmpName.rfind('/'));

  for (WiiFile* f : m_children) {
    if (f->isFile())
      continue;

    WiiFile* ret = f->child(tmpName);

    if (ret)
      return ret;
  }

  return nullptr;
}

void WiiFile::removeChild(WiiFile* file) {
  std::vector<WiiFile*>::iterator iter = std::find(m_children.begin(), m_children.end(), file);

  if (iter == m_children.end())
    return;

  m_children.erase(iter);
}

WiiFile* WiiFile::parent() { return m_parent; }

void WiiFile::setParent(WiiFile* parent) {
  if (m_parent)
    m_parent->removeChild(this);

  m_parent = parent;
  m_parent->addChild(this);
}

atUint32 WiiFile::fileCount() {
  size_t ret = m_children.size();

  for (WiiFile* f : m_children) {
    if (f->isFile())
      continue;

    ret += f->fileCount();
  }

  return (atUint32)ret;
}

std::vector<WiiFile*> WiiFile::allChildren() {
  std::vector<WiiFile*> ret;

  if (m_children.size() == 0)
    return ret;

  // Add our children first
  for (WiiFile* f : m_children)
    ret.push_back(f);

  // now lets add our children's children
  for (WiiFile* f : m_children) {
    if (f->isFile())
      continue;

    std::vector<WiiFile*> tmp = f->allChildren();

    if (tmp.size() == 0)
      continue;

    ret.insert(ret.end(), tmp.begin(), tmp.end());
  }

  return ret;
}

std::string WiiFile::fullpath() {
  std::string ret;

  if (m_parent)
    ret = m_parent->filename() + "/";

  ret = ret + filename();

  while (ret.at(0) == '/')
    ret.erase(ret.begin());

  return ret;
}

} // namespace athena
