# Copyright 1999-2014 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=5

inherit eutils

DESCRIPTION="A library for configuring nginx"
HOMEPAGE="https://github.com/tgockel/nginxconfig"
SRC_URI="https://github.com/tgockel/${PN}/archive/v${PV}.tar.gz -> ${PN}-v${PV}.tar.gz"
CATEGORY="dev-libs"

LICENSE="Apache"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE=""

RDEPEND=""
DEPEND="${RDEPEND}
        >=dev-libs/boost-1.52"

src_configure()
{
    export NGINXCONFIG_VERSION=${PV}
    export USE_BOOST_REGEX=0
}

src_install()
{
    emake DESTDIR="${D}" install
}
