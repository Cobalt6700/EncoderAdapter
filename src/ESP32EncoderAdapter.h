/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef ESP32EncoderAdapter_h
#define ESP32EncoderAdapter_h

#define ESP32Encoder_h_

// Only create the adapter class if ESP32 Encoder is already included.
#ifdef ESP32Encoder_h_

#include <ESP32Encoder.h> // https://github.com/madhephaestus/ESP32Encoder.git 

/**
 * @brief An adapter for Kevin Harringtons ESP32 Encoder.
 * 
 * @details Uses the ESP32 pulse counter hardware peripheral. Support for ESP32 and ESP32c2, ESP32s3 has just 2 PCNT modules, so only supports 2 hardware accelerated encoders.
 * 
 */
class ESP32EncoderAdapter : public EncoderAdapter {

public:

    /**
     * @brief Construct a new ESP32 Encoder Adapter object
     * 
     * @param encoderPin1 The first encoder pin (sometimes known as pin A)
     * @param encoderPin2 The second encoder pin (sometimes known as pin B)
     */
    ESP32EncoderAdapter( uint8_t encoderPin1, uint8_t encoderPin2 )
        {
            pinA = encoderPin1;
            pinB = encoderPin2;
            encoder = new ESP32Encoder();
        };

    /**
     * @brief Destructor to ensure the ESP32 encoder is deleted.
     * 
     */
    ~ESP32EncoderAdapter() {
        delete encoder;
    }


    /**
     * @brief In Encoder < v1.4.4 this is not used but when v1.4.5 is released we will use it to initialise the encoder in setup for better compatibility with some boards.
     * 
     * @return true 
     * @return false 
     */
    bool begin(void) {
        // ESP32's Encoder appears to have begin() functionality on its way but something has gone wrong with the versioning 
        // https://github.com/PaulStoffregen/Encoder/issues/106#issuecomment-2621583974
        //encoder->begin(pinA, pinB);
        // encoder = new Encoder(pinA, pinB); 
        ESP32Encoder::useInternalWeakPullResistors = puType::up;
        encoder.attachFullQuad( pinA, pinB );
        encoder.setFilter(1023);
        encoder.setCount ( 0 );
        return true;
    }

    /**
     * @brief Get the current position as reported by the Encoder class.
     * 
     * @return int32_t The position. May be positive or negative.
     */
    int32_t getPosition(void) {
        // if ( counts > oldCounts ) {
        //     if (counts == ( oldCounts + enc_div )) {
        //         newPosition = counts / enc_div;
        //         oldPosition = newPosition;
        //         oldCounts = counts;
        //     }
        // }
        return encoder->getCount() / enc_div;
    }

    /**
     * @brief Set the a new position of the encoder. This can be any value in the int32_t range.
     * 
     * @param pos The desired new position.
     */
    void setPosition(int32_t pos) {
        encoder->setCount(pos * enc_div);
    }

private:


    Encoder *encoder;

    uint8_t pinA;
    uint8_t pinB;

    int8_t enc_div = 4;
    int32_t counts = 0;
    int32_t oldCounts = 0;
    int32_t newPosition = 0;
    int32_t oldPosition = 0;

};



#endif
#endif
