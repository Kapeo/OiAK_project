do poprawnosci działania testu potrzebny jest biblioteka boosta:
na linuxie wystarczy wykonać komendę sudo apt-get install libboost-test-dev

kompilujemy test w nastepujący sposób:

g++ -o test test.cpp -lboost_unit_test_framework


wyniki testu:

./test --log_level=test_suite
