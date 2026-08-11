#include "ch32v003fun.h"

#define SILENCE_TIMEOUT_MS 250

volatile uint32_t last_audio_pulse = 0;
volatile uint8_t audio_present = 0;

typedef enum {
    MODE_BLINKY = 0,
    MODE_RDF_PROXIMITY,
    MODE_RDF_RADAR,
    MODE_RDF_MINIMAL
} BadgeState_t;

BadgeState_t current_mode = MODE_BLINKY;

// Timer Capture ISR triggered by incoming 500Hz Doppler tone pulses
void TIM1_CC_IRQHandler(void) __attribute__((interrupt));
void TIM1_CC_IRQHandler(void) {
    if (TIM1->STATR & TIM_STATR_CC1IF) {
        TIM1->STATR = ~TIM_STATR_CC1IF; // Clear flag
        
        last_audio_pulse = millis();
        
        // Auto-switch from Blinky Mode to Active Hunt Mode on connection!
        if (!audio_present) {
            audio_present = 1;
            if (current_mode == MODE_BLINKY) {
                current_mode = MODE_RDF_PROXIMITY;
                beep_buzzer_short(); // Audible confirmation of lock
            }
        }
    }
}

void process_audio_timeout(void) {
    if (audio_present && (millis() - last_audio_pulse > SILENCE_TIMEOUT_MS)) {
        audio_present = 0;
        // Revert back to Blinky Mode when unplugged
        current_mode = MODE_BLINKY; 
    }
}

int main(void) {
    hardware_init();
    
    while (1) {
        process_audio_timeout();
        check_button_presses(); // Manual mode override
        
        switch (current_mode) {
            case MODE_BLINKY:
                run_random_blinky_animation();
                break;
                
            case MODE_RDF_PROXIMITY:
            case MODE_RDF_RADAR:
            case MODE_RDF_MINIMAL:
                run_rdf_bearing_and_proximity(current_mode);
                break;
        }
    }
}