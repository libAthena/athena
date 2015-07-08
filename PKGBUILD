# PKGBUILD for libAthena
_pkgname=libathena
pkgname=$_pkgname-git
pkgver=1.1.0.59.g582d2ce
pkgrel=1
pkgdesc="Basic cross platform IO library"
arch=('i686' 'x86_64')
source=("${pkgname%-*}::git+https://github.com/libAthena/Athena.git#branch=copy-streamers")
options=(staticlibs)
license="MIT"
makedepends=('git cmake sed')
md5sums=('SKIP')
sha256sums=('SKIP')

pkgver() {
    cd "$srcdir/$_pkgname"
    git describe --tags | sed 's|-|.|g'
}

build() {
    cd "$srcdir/$_pkgname"
    cmake -DCMAKE_INSTALL_PREFIX="$pkgdir/usr"
    make
}

package() {
    cd "$srcdir/$_pkgname"
	make install
}

