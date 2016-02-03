Thanks for trying my lib. It has been quite a while, and there are still people email me about how to use it. I really appreciate that.

I have written (an blog about hx711)[http://aguegu.net/?p=1327], well, in Chiese...

To config the module right, you need to config the offset and the scale. The `setScale` function should be named `setRatio` to make its function more clear.

The offset and scale value are got by experiments, which means they vary according to different setups.

To do the test right:

1. place nothing on the scale, the averageValue() should be the value for setOffset(). Then getGram() should be 0.
2. place a stand weight, like 1kg(1000g), calculate ratio = (averageValue() - offset) / 1000, and setScale with the ratio value. Then getGram should be the stand weight value, like 1000.

the averageValue() basically is the raw output. use it to calculate the offset and scale(ratio),

```
getGram() = (averageValue() - offset) / ratio;
```

Just middle school math. :)
