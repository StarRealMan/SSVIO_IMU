#include "imu_update.h"

extern FP32 Gyro_X_Real;
extern FP32 Gyro_Y_Real;
extern FP32 Gyro_Z_Real;

extern FP32 Acc_X_Real;
extern FP32 Acc_Y_Real;
extern FP32 Acc_Z_Real;

float Kp = 0.5f;
float Ki = 0.001f;

float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;

static float q0 = 1.0f;	
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;

float quaternion_0 = 1.0f;
float quaternion_1 = 0.0f;
float quaternion_2 = 0.0f;
float quaternion_3 = 0.0f;

void IMU_Update_Mahony(_imu_st *imu,float dt)	
{
		float normalise;
    float nor_acc[VEC_XYZ] = {0};
    float ex, ey, ez;
    float q0s, q1s, q2s, q3s;
    static float R11,R21;
    static float vecxZ, vecyZ, veczZ;
    float half_T = 0.5f * dt;

    float q0Last = q0;
    float q1Last = q1;
    float q2Last = q2;
    float q3Last = q3;
    float delta_theta[3];
    float delta_theta_s;

    if((Acc_X_Real != 0.0f) || (Acc_Y_Real != 0.0f) || (Acc_Z_Real != 0.0f))
    {
        nor_acc[X] = Acc_X_Real;
        nor_acc[Y] = Acc_Y_Real;
        nor_acc[Z] = Acc_Z_Real;

        /*  */
        normalise = invSqrt(nor_acc[X] * nor_acc[X] + nor_acc[Y] * nor_acc[Y] + nor_acc[Z] * nor_acc[Z]);
        nor_acc[X] *= normalise;
        nor_acc[Y] *= normalise;
        nor_acc[Z] *= normalise;

        ex = (nor_acc[Y] * veczZ - nor_acc[Z] * vecyZ);
        ey = (nor_acc[Z] * vecxZ - nor_acc[X] * veczZ);
        ez = (nor_acc[X] * vecyZ - nor_acc[Y] * vecxZ);

        exInt += Ki * ex * dt ;
        eyInt += Ki * ey * dt ;
        ezInt += Ki * ez * dt ;

        Gyro_X_Real += Kp * ex + exInt;
        Gyro_Y_Real += Kp * ey + eyInt;
        Gyro_Z_Real += Kp * ez + ezInt;
    }

    delta_theta[0] = Gyro_X_Real*half_T;
    delta_theta[1] = Gyro_Y_Real*half_T;
    delta_theta[2] = Gyro_Z_Real*half_T;
    delta_theta_s = delta_theta[0]*delta_theta[0] + delta_theta[1]*delta_theta[1] + delta_theta[2]*delta_theta[2];

    q0 = q0Last*(1-delta_theta_s) - q1Last * delta_theta[0] - q2Last * delta_theta[1] - q3Last * delta_theta[2];
    q1 = q1Last*(1-delta_theta_s) + q0Last * delta_theta[0] + q2Last * delta_theta[2] - q3Last * delta_theta[1];
    q2 = q2Last*(1-delta_theta_s) + q0Last * delta_theta[1] - q1Last * delta_theta[2] + q3Last * delta_theta[0];
    q3 = q3Last*(1-delta_theta_s) + q0Last * delta_theta[2] + q1Last * delta_theta[1] - q2Last * delta_theta[0]; 

    normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= normalise;
    q1 *= normalise;
    q2 *= normalise;
    q3 *= normalise;

    q0s = q0 * q0;
    q1s = q1 * q1;
    q2s = q2 * q2;
    q3s = q3 * q3;

    R11 = q0s + q1s - q2s - q3s;
    R21 = 2 * (q1 * q2 + q0 * q3);
    
    vecxZ = 2 * (q1 * q3 - q0 * q2);
    vecyZ = 2 * (q0 * q1 + q2 * q3);
    veczZ = q0s - q1s - q2s + q3s;

    if (vecxZ>1) vecxZ=1;
    if (vecxZ<-1) vecxZ=-1;

    imu->pit = -asinf(vecxZ) * 57.30f;
    imu->rol = atan2f(vecyZ, veczZ) * 57.30f;
    imu->yaw = atan2f(R21, R11) * 57.30f;
		
		quaternion_0 = q0;
		quaternion_1 = q1;
		quaternion_2 = q2;
		quaternion_3 = q3;
}

	 
