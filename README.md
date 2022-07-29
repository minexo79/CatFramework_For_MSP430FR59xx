# CatFramework_For_MSP430FR59xx
CatFramework - 讓430能更簡單開發

![](https://img.shields.io/github/last-commit/minexo79/CatFramework_For_MSP430FR59xx)
![](https://img.shields.io/github/languages/code-size/minexo79/CatFramework_For_MSP430FR59xx?color=yellow&style=flat-square)
![](https://img.shields.io/github/issues/minexo79/CatFramework_For_MSP430FR59xx)
![](https://img.shields.io/github/issues-pr/minexo79/CatFramework_For_MSP430FR59xx)
![](https://img.shields.io/github/forks/minexo79/CatFramework_For_MSP430FR59xx?style=social)
![](https://img.shields.io/github/stars/minexo79/CatFramework_For_MSP430FR59xx?style=social)

> 因現實關係，目前維護時間有限，後續會將此文檔及框架慢慢完善。

# 前言
哈囉，當你看到這份文件，這是我原本要讓自己能更容易開發MSP430，將幾個常用的變數及功能整合起來，變成一個讓自己更方便開發的個人函式庫。<br>
但幾年大學的研究下來，發現到光有函式庫是不夠的，原因在於：功能不會只有一種，而是會根據需求做變化。<br>
於是某天決定開始動手並整理此函式庫，並用我在網路社群活躍的暱稱，取名叫「黑貓框架（CatFramework）」。<br>

# 介紹
這個框架旨在讓開發者不用透過暫存器（Register）設定430的某些單元。<br>
使用這個框架，你能得到類似Arduino方便的功能。比如下面透過暫存器設定UART的程式。
```c
// UART Initialize
P2SEL0 &= ~(BIT0 | BIT1);                  // P2.0: TX. P2.1: RX
P2SEL1 |= (BIT0 | BIT1);

UCA0CTLW0 |= (UCSWRST | UCSSEL__SMCLK);    // Hold UART, UARTCLK: SMCLK

UCA0BR0 = 104;                             // 1Mhz 9600bps
UCA0BR1 = 0;

UCA0CTLW0 &= ~(UCSWRST);                   // Initialize UART State Machine

UCA0IE |= UCRXIE;                          // Rx Interrupt
```
但如果改用此框架，只需輸入一行即可完成設定UART單元。
```c
Initialize_UART(UART0, 9600, NONIE);       // Configure UART0, 9600 bps, disable TX/RX Interrupt
```

# 支援的單元
- [x] SystemClock
- [x] GPIO（僅有Initialize Pin功能）
- [x] Timer（僅支援TimerAx單元）
- [ ] ADC
- [x] UART
- [ ] I2C
- [ ] SPI
- [ ] DMA
