# PKGBUILD for libAthena
_pkgname=libathena
pkgname=$_pkgname-git
pkgver=1.1.0.17.gfb722a9
pkgrel=1
pkgdesc="Basic cross platform IO library"
arch=('i686' 'x86_64')
source=("${pkgname%-*}::git+https://github.com/Antidote/Athena.git#branch=atdna")
options=(staticlibs)
license="MIT"
makedepends=('git qt5-base sed')
md5sums=('SKIP')
sha256sums=('SKIP')

pkgver() {
    cd "$srcdir/$_pkgname"
    git describe --tags | sed 's|-|.|g'
}

build() {
    cd "$srcdir/$_pkgname"
    qmake && make
}

package() {
    cd "$srcdir/$_pkgname"
	qmake PREFIX="$pkgdir/usr" && make install
}

