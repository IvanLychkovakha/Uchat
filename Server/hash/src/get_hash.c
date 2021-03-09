#include "../inc/hash.h"

void get_hash(t_hash *hash, char* user_string, unsigned int lenght_hash){
    if(lenght_hash > 3){
        //Минимальая длина строки хеш, кратна двум
        unsigned int minLen = 2;
        //Длина строки, ближайшая к длине хеша
        unsigned int realMinLen = 0;
        //Длина для добавления

        int len_user_string = strlen(user_string);

        //Получение соль оригинальной строки
        unsigned int originalSault = get_control_sum(user_string);
        unsigned int originalLenghtStr = mx_strlen(user_string);
        

        //Получение длины строки, кратной степени двух, ближайший к заданой длине хеша
        while (minLen <= lenght_hash)
            realMinLen = (minLen *= 2);

        //Получаем бдижайшее к длине исходной строки число такого типа  - 2ˆn
        while (minLen < originalLenghtStr)
            minLen *= 2;

        //Делаем длину строки хеша, как минимум, в 2 раза длиней оригинальной строки
        if((minLen - originalLenghtStr) < minLen)
            minLen *= 2;

        //Получаем количество символов, которые необходимо добавить к строке
        int addCount = minLen - originalLenghtStr;

        //Добавление
        for (int i = 0; i < addCount; i++)
            user_string[len_user_string++] = receiving_exist_codes(user_string[i] + user_string[i+1]);

        user_string[len_user_string] = '\0';

        //получаем максимальную соль
        int maxSault = get_control_sum(user_string);
        int maxLenghtStr = mx_strlen(user_string);

        //определение степени сжатия

        while(maxLenghtStr != realMinLen) {
            for(int i = 0, center = maxLenghtStr / 2; i < center; i++) {
                hash->str[i] = receiving_exist_codes(user_string[center - i] + user_string[center + i]);
            }
            strcpy(user_string, hash->str);
            bzero(hash->str, MAX_HASH_LEN);
        }

        //Приведение к нужной длине
        unsigned int rem = realMinLen - lenght_hash;
        int j = 0;

        for (unsigned int i = 0, countCompress = realMinLen / rem; strlen(hash->str) < (lenght_hash - 4); i++) {
            if(i % countCompress == 0) {
                int k = user_string[i] + user_string[i+1];
                i++;
                hash->str[j] = receiving_exist_codes(k);

            }
            else 
                hash->str[j] = user_string[i];

            j++;
        }

        //добавление оригинальных солей
        hash->str[j++] = receiving_exist_codes(originalSault);
        hash->str[j++] = receiving_exist_codes(originalLenghtStr);

        //Добавление максимальных солей
        hash->str[j++] = receiving_exist_codes(maxSault);
        hash->str[j++] = receiving_exist_codes(maxLenghtStr);

        hash->str[j] = '\0';
        return; 
    }
}
