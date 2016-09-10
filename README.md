php-postal
--------

[![Build Status](https://travis-ci.org/openvenues/php-postal.svg?branch=master)](https://travis-ci.org/openvenues/php-postal)

These are the official PHP bindings to [libpostal](https://github.com/openvenues/libpostal), a C library for fast international street address parsing and normalization.

Usage
-----

```php
$expansions = Postal\Expand::expand_address("Quatre vingt douze Ave des Champs-Élysées");
foreach ($expansions as $expansion) {
    echo "$expansion\n";
}

$parsed = Postal\Parser::parse_address("The Book Club 100-106 Leonard St, Shoreditch, London, Greater London, EC2A 4RH, United Kingdom");
foreach ($parsed as $component) {
    echo "{$component['label']}: {$component['component']}\n";
}
```

Installation
------------

Before using the PHP bindings, you must install the libpostal C library. Make sure you have the following prerequisites:

**On Ubuntu/Debian**
```
sudo apt-get install curl libsnappy-dev autoconf automake libtool pkg-config
```

**On CentOS/RHEL**
```
sudo yum install snappy snappy-devel autoconf automake libtool pkgconfig
```

**On Mac OSX**
```
sudo brew install snappy autoconf automake libtool pkg-config
```

**Installing libpostal**

```
git clone https://github.com/openvenues/libpostal
cd libpostal
./bootstrap.sh
./configure --datadir=[...some dir with a few GB of space...]
make
sudo make install

# On Linux it's probably a good idea to run
sudo ldconfig
```

**Installing the PHP extension**

```
git clone https://github.com/openvenues/php-postal
cd php-postal
phpize
./configure
make
sudo make install
```

This will build a 'postal.so' shared extension. Add it to php.ini using:

```
[postal]
extension = postal.so
```

Compatibility
-------------

php-postal currently supports PHP 7. Please see previous versions for PHP 5.5 and 5.6 support. HipHop/HHVM uses a different process for extensions (C++) so would be out of scope for this repo.

Tests
-----

After compiling, run:

```
make test
```
