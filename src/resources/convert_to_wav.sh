for file in `find texts -type f -name "*.wav"`
do
   filename=$(basename -- "$file")
   # % удаляет кратчайшее вхождение указанной строки с конца переменной
   # поддерживает шаблоны подстановочных знаков, например, *)
   name=${filename%.*}
   $(ffmpeg -i "${file}" -acodec pcm_s16le -ac 1 -ar 44100 "texts_wav/$name.wav")
done
