# PKGBUILD for libAthena
_pkgname=Athena
pkgname=$_pkgname-git
pkgver=v1.0.0
pkgrel=1
pkgdesc="Basic cross platform IO library"
arch=('i686' 'x86_64')
source=("${pkgname%-*}::git+https://github.com/Antidote/Athena.git")
license="GPLv3"
makedepends=('git qt5-base sed')
md5sums=('SKIP')
sha256sums=('SKIP')

pkgver() {
    cd "$srcdir/$_pkgname"
    git describe --tags | sed 's|-|.|g'
}

build() {
    cd "$srcdir/$_pkgname"
    qmake PREFIX="$pkgdir/usr"
    make
}

package() {
    cd "$srcdir/$_pkgname"
    make install
}

