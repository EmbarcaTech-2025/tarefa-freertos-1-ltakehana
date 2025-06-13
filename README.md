
# Tarefa: Roteiro de FreeRTOS #1 - EmbarcaTech 2025

Autor: Leonardo Takehana

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília , junho de 2025

---

## 1. Descrição do Projeto

Este projeto implementa um sistema multitarefa embarcado utilizando a placa BitDogLab (baseada no Raspberry Pi Pico W), programado em linguagem C com FreeRTOS no ambiente VSCode. O sistema controla de forma concorrente três periféricos:

- Um **LED RGB** que alterna ciclicamente entre as cores vermelho, verde e azul a cada 500 ms.
- Um **buzzer** que emite bipes periódicos (beep curto a cada 1 segundo).
- Dois botões:
  - **Botão A**: suspende ou retoma a tarefa do LED RGB.
  - **Botão B**: suspende ou retoma a tarefa do buzzer.

---

## 2. Objetivos de Aprendizagem

- Criar e gerenciar múltiplas tarefas usando FreeRTOS.
- Compreender escalonamento e prioridades em RTOS.
- Utilizar os comandos `vTaskCreate()`, `vTaskSuspend()` e `vTaskResume()`.
- Controlar GPIOs com FreeRTOS.
- Organizar projetos no VSCode com pico-sdk e FreeRTOS.

---

## 3. Materiais e Pré-requisitos

- Placa BitDogLab (Raspberry Pi Pico W ou Pico 2).
- Cabo micro-USB ou USB-C.
- Ambiente configurado: VSCode, ARM GCC Toolchain, CMake, pico-sdk, FreeRTOS Kernel.

---

## 4. Estrutura do Projeto

```
TAREFA-FREERTOS-1-LTAKEHANA/
├── CMakeLists.txt
├── pico_sdk_import.cmake
├── FreeRTOS/
├── src/
│   └── main.c
├── include/
│   └── FreeRTOSConfig.h
```

---

## 5. Implementação

- Tarefa LED RGB: alterna entre vermelho, verde e azul a cada 500ms.
- Tarefa Buzzer: emite beep curto com PWM a cada 1 segundo.
- Tarefa Botões: faz polling dos botões a cada 100ms para suspender/retomar as tarefas.

Principais funções FreeRTOS usadas:

- `vTaskCreate()`, `vTaskDelay()`, `vTaskSuspend()`, `vTaskResume()`.

---

## 6. Compilação e Gravação

```bash
mkdir build
cd build
cmake ..
make -j4
```

Copie o arquivo `.uf2` para a placa em modo bootloader.

---

## 7. Reflexão

### 7.1 O que acontece se todas as tarefas tiverem a mesma prioridade?

Se todas as tarefas tiverem a mesma prioridade, o FreeRTOS fará um escalonamento cooperativo com timeslicing entre elas, dividindo o tempo da CPU de forma justa. Isso pode funcionar para esse projeto simples, mas tarefas que exigem maior tempo de processamento podem sofrer atraso.

### 7.2 Qual tarefa consome mais tempo da CPU?

Provavelmente a tarefa dos botões consome mais CPU, pois faz polling a cada 100 ms e verifica estados, enquanto as tarefas do LED e buzzer ficam bloqueadas durante os delays. Porém, o consumo total ainda é baixo.

### 7.3 Quais seriam os riscos de usar polling sem prioridades?

Polling contínuo sem prioridades pode causar desperdício de CPU, maior consumo de energia e resposta lenta a eventos críticos, além de dificultar a escalabilidade do sistema. A ausência de prioridade pode fazer com que tarefas importantes sejam atrasadas ou não executadas a tempo.

---

## 📜 Licença
GNU GPL-3.0.
