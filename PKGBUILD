# Maintainer: PolyDung Dev <polydung.dev@proton.me>

pkgname=wolfram
pkgver=0.4.1
pkgrel=1
pkgdesc='An elementary cellular automata module for python.'
arch=('x86_64')
url="https://github.com/polydung-dev/${pkgname}"
license=('MIT')
depends=(
	'glibc'
)
makedepends=(
	'perl'
	'python'
	'python-setuptools'
)
source=(
	"${pkgname}-${pkgver}.tar.gz::https://github.com/polydung-dev/${pkgname}/archive/refs/tags/v${pkgver}.tar.gz"
	'str.c'
	'str.h'
	'wolframmodule.c'
	'setup.py'
)
sha256sums=('ffb4ed662a5d8d1b6443a533bc3abea0b707f5f4c81c7029f644b20fbe2645ea'
            '6cb6cdde7e988da3b93f9bf119e4b23eaedb3301c782f71c1106214dd0d0d4b5'
            '81f300e8b01076e2a9d6b99bafa2c42faf7d93b4de8753d1c76fd56eb5afb298'
            '5d3704a6367f5f56158f128fdb579a6ecc5bb61b75197e948c7867601539f610'
            'f4750116f82e9fb0ac3d6420581861c59dd1399cee3fb19b13aed354c07b28cc')

prepare() {
	mv wolframmodule.c str.c str.h "${pkgname}-${pkgver}/src/"
	mv setup.py "${pkgname}-${pkgver}/"
}

build() {
	cd "${pkgname}-${pkgver}"
	python ./setup.py build
}

package() {
	cd "${pkgname}-${pkgver}"
	libpath=$(find "build" -name *.so)
	libname=$(basename ${libpath})
	pyver=$(echo ${libname} | perl -pe "s/.*cpython-(\d)(\d\d).*/\1.\2/")
	install -Dm755 "${libpath}" "$pkgdir/usr/lib/python$pyver/site-packages/${libname}"
	install -Dm755 LICENSE "$pkgdir/usr/share/licenses/${pkgname}/LICENSE"
}
