cd /var/www/php-src/ext/vary-php && sudo make clean
cd /var/www/php-src/ext/vary-php && sudo make
cd /var/www/php-src/ext/vary-php && sudo make install
sudo service php7.2-fpm restart
npm run test