# Programming

## Arduino

Inside Arduino:

```c
main() // starting point of execution
{
  setup(); // initialization, runs only once
  while (true)
  {
    loop(); // runs forever, no initialization here
  }
}
```

---

What we see:

```c
void setup()
{
}

void loop()
{
}
```

[Go to Blink](Blink.md)
