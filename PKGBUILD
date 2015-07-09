# PKGBUILD for libAthena
_pkgname=libathena
pkgname=$_pkgname-git
pkgver=1.1.0.70.g1e55ce5
pkgrel=1
pkgdesc="Basic cross platform IO library"
arch=('i686' 'x86_64')
source=("${pkgname%-*}::git+https://github.com/libAthena/Athena.git")
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
    mkdir -p build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX="$pkgdir/usr" ..
    make
}

package() {
    cd "$srcdir/$_pkgname/build"
	make install
}

