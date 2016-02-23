Thanks for trying my lib. It has been quite a while, and there are still people email me about how to use it. I really appreciate that.

I have written [an blog about hx711](http://aguegu.net/?p=1327), well, in Chiese...

To config the module right, you need to config the offset and the scale. The `setScale` function should be named `setRatio` to make its function more clear. But for now, just leave it the way it is.

The offset and scale value are got by experiments, which means they vary according to different setups.

To do the test right:
```cpp
#include "hx711.h"

Hx711 scale(A1, A0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(scale.averageValue());
  delay(200);
}
```

1. place nothing on the scale, then run the example code, which means reset your arduino that runs the example code above.

record the output. that is the `offset`. 

2. place a stand weight, like 1kg(1000g), record the output as `w`.

3. Do the math 

ratio = (`w` - `offset`) / 1000

4. modify your application code to  

```cpp
#include "hx711.h"

Hx711 scale(A1, A0);

void setup() {
  Serial.begin(9600);
  scale.setOffset(fill in the `offset` value here);
  scale.setScale(fill in the `ratio` value here);
}

void loop() {
  Serial.println(scale.getGram());
  delay(200);
}
```

Then arduino should give you the right result in unit gram.

The init program has set the average value for the first few runs to the offset, with the function `setOffset`. It is written in the lib. 

My example code is supposed to work with scale that got a few seconds of zero weight when the program start running. But we still need to do the test to calculate the ratio. It is just happen to be 742 in my setup, but it dose not fit for other setups. 

If the setup got weight already when the program start running, and you do not want its getGram() output to be zero. Then you have to setOffset() again manually with the tested value to override the auto offset, just like the example code above.

the averageValue() basically is the raw output. It is not affected by the offest or the scale(ratio). The offset determind your zero point. while the scale(ratio) determind what unit you want the getGram() output to be. With different ratio value, the output can be in g, kg, oz, pound, whatever, as long as the value in the range of the setup and makes sense to you.

```
0g = 0kg = 0oz = 0pound
1000g = 1kg = 35.274oz = 2.20462 pound
```

Just remember that the raw output and the calculated (`getGram()`) output, are linear related.


```
getGram() = (averageValue() - offset) / ratio;
```

Just middle school math. :)
