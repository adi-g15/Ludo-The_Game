pkgname=ludo-the-game-git
pkgver=r135.b6c73fc
pkgrel=1
pkgdesc="An implementation of the famous board game Ludo, using C++"
arch=('x86_64')
url="https://github.com/adi-g15/Ludo-The_Game"
license=('MIT')
depends=()
makedepends=('git' 'cmake' 'gcc')
provides=("${pkgname%-git}")
conflicts=("${pkgname%-git}")
source=('ludo-the-game::git+https://github.com/adi-g15/Ludo-The_Game')
md5sums=('SKIP')

pkgver() {
        cd "$srcdir/${pkgname%-git}"

	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
	cd "$srcdir/${pkgname%-git}"
        mkdir -p build && cd build
        rm -f CMakeCache.txt
	cmake .. -DCMAKE_BUILD_TYPE=Release
	cmake --build .
}

package() {
	cd "$srcdir/${pkgname%-git}"
        cd build
	DESTDIR="$pkgdir/" cmake --install .
}
