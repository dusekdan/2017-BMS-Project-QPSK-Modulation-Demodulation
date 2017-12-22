# Additional testing data

Data originally generated and provided Tomáš Bártů ([@bartut](https://github.com/bartut)).

To test the **moduler**, just put together script that will run `./bms2A` over all files within `txt` directory. Modulated signal will be generated to .wav files located at the directory, so you will not override reference outputs. Then you can inspect your modulated .wav files and wav files from `wav` directory, for example using Audacity software. It should look exactly the same if you use 30 samples to modulate one baud, or they should look very similar if you use other value.

Same process applies for testing **demoduler** except you run `./bms2B` over all files within `wav` directory. Then you should be able to easily `diff` generated .txt files from terminal.


