FROM ubuntu:18.04
ENV MYSQL_PWD 123
RUN echo "mysql-server mysql-server/root_password password $MYSQL_PWD" | debconf-set-selections
RUN echo "mysql-server mysql-server/root_password_again password $MYSQL_PWD" | debconf-set-selections
RUN \
  apt-get update && \
  apt-get install -y sudo git nano software-properties-common curl nginx
RUN sudo apt-get install -y mysql-server
RUN LC_ALL=C.UTF-8 sudo add-apt-repository -y ppa:ondrej/php
ENV DEBIAN_FRONTEND=noninteractive
RUN \
  apt-get update && \
  apt-get install -y php7.2-cli php7.2-dev php7.2-fpm php7.2-mysql php7.2-curl valgrind
RUN curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash - && sudo apt-get install -y nodejs
RUN sudo npm install -g npm@latest
RUN mkdir -p /var/www
RUN git clone https://github.com/php/php-src.git
RUN mkdir -p /var/www/php-src/ext/vary-php/
COPY . /var/www/php-src/ext/vary-php/
WORKDIR /var/www/php-src/ext/vary-php
RUN sudo cp helpers/nginx /etc/nginx/sites-available/default
ENTRYPOINT \
  cp /var/www/php-src/ext/vary-php/helpers/php.ini /etc/php/7.2/cli/php.ini && \
  cp /var/www/php-src/ext/vary-php/helpers/php.ini /etc/php/7.2/fpm/php.ini && \
  sudo chown -R mysql:mysql /var/lib/mysql && service mysql start && \
  mysql --user=root --password=123 -e "CREATE DATABASE IF NOT EXISTS test" && \
  mysql --user=root --password=123 test < /var/www/php-src/ext/vary-php/helpers/seeder.sql && \
  cd /var/www/php-src/ext/vary-php && phpize && \
  cd /var/www/php-src/ext/vary-php && ./configure && \
  sudo service php7.2-fpm start && \
  sudo service nginx start && \
  bash
