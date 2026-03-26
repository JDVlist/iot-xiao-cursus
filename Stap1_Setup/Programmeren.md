# Programmeren

## Arduino

Arduino intern:

``` c
main() // begin punt van uitvoering
{
  setup(); // initialisatie, wordt maar 1 keer uitgevoerd
  while (true)
  {
    loop(); // loopt voor altijd, geen initialisaties hier
  }
}
```

---

Wat wij zien:

```c
void setup()
{
}

void loop()
{
}
```

[Ga naar Blink](Blink.md)
