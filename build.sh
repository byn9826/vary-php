sudo cp ~/Code/vary-php /var/www/php-src/ext -R
cd /var/www/php-src/ext/vary-php && sudo make clean
cd /var/www/php-src/ext/vary-php && sudo make
cd /var/www/php-src/ext/vary-php && sudo make install
npm run test