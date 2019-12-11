
const char *cook_vaihe_texts[NUM_COOKING_VAIHEET] =
{
   "COOKING_OFF",
   "COOKING_VAIPAN_TYHJENNYS_ALUKSI", //timeout:illa
   "COOKING_VAIPPAVEDEN_TAYTTO", //täytetään uimuriin asti ennenkuin vastukset voi laittaa päälle
   "COOKING_VAIPPAVESI_OK",
   "COOKING_READY",
};

const char *chill_vaihe_texts[NUM_CHILLING_VAIHEET] =
{
   "CHILLING_OFF",
   "CHILLING_WAIT_IF_100C", //chill alkaa vasta kun vaippavesi alle 100C
   "CHILLING_2_VAIHE_CHILL_VAIHE_1",
   "CHILLING_2_VAIHE_CHILL_VAIHE_2",
   "CHILLING_1_VAIHE_CHILL",
   "CHILLING_READY",
};

const char *empty_vaihe_texts[NUM_EMPTYING_VAIHEET] =
{
   "EMPTYING_OFF",
   "EMPTYING_WAIT_IF_55C", // Tyhjennys estetty jos vaipan lämpö >55
   "EMPTYING_GOING",
   "EMPTYING_TIMED_OUT",
   "EMPTYING_READY",
};

const char *water_fill_vaihe_texts[NUM_WATER_FILL_VAIHEET] =
{
   "WATER_FILL_OFF",
   "WATER_FILL_MITTAA_VESIMAARA",
   "WATER_FILL_MITTAA_VESIMAARA_HOT",
   "WATER_FILL_VESIMAARA_OK",
   "WATER_FILL_ERROR",
};

const char *mixer_motor_vaihe_texts[NUM_MIXER_MOTOR_VAIHEET] =
{
   "MIXER_MOTOR_STOPPED",
   "MIXER_MOTOR_RAMP_ETEEN",
   "MIXER_MOTOR_RUN_ETEEN",
   "MIXER_MOTOR_RAMP_TAAKSE",
   "MIXER_MOTOR_RUN_TAAKSE",
   "MIXER_MOTOR_IN_PAUSE",
   "MIXER_MOTOR_PULSED_STATE",
   "MIXER_MOTOR_ERROR",
};

const char *tilt_motor_vaihe_texts[NUM_TILT_MOTOR_VAIHEET] =
{
   "TILT_MOTOR_STOPPED",
   "TILT_MOTOR_RAMP_UP",
   "TILT_MOTOR_RUN_UP",
   "TILT_MOTOR_RAMP_DOWN",
   "TILT_MOTOR_RUN_DOWN",
   "TILT_MOTOR_ERROR",
};