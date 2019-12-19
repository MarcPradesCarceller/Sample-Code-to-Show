using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "Enemy", menuName = "Project/Enemy")]
public class EnemyModifier : ScriptableObject {

    public GameObject enemy;
    public bool edit;

    public float speedMax;
    public float speedMin;

    public float changeSpeed;
    public float changeTimeMax;
    public float changeTimeMin;

    public Color attakColor = Color.red;

    public bool isKamikaze;
    public float kamikazeTimeMax;
    public float kamikazeTimeMin;
    public float kamikazeSpeed;

    public bool fireBullets;
    public float shootTimeMax;
    public float shootTimeMin;
    public float waitingBeforeShoot;
    public float waitingAfterShoot;

    public int pointsForKilling;

 


    private EnemyAxisMovement movement;
    private EnemyAttack attack;
    private bool hasGeted = false;

    public void GetArguments()
    {
        if(enemy != null)
        {
            if (!hasGeted)
            {
                movement = enemy.GetComponentInChildren<EnemyAxisMovement>();
                attack = enemy.GetComponentInChildren<EnemyAttack>();

                speedMax = movement.MaxSpeed;
                speedMin = movement.MinSpeed;
                changeSpeed = movement.ChangeSpeed;
                changeTimeMax = movement.MaxChangeTime;
                changeTimeMin = movement.MinChangeTime;

                attakColor = attack.AttackColor;

                isKamikaze = attack.IsKamikaze;
                kamikazeTimeMax = attack.MaxKamikazeTime;
                kamikazeTimeMin = attack.MinKamikazeTime;
                kamikazeSpeed = attack.KamikazeSpeed;

                fireBullets = attack.FireBullets;
                shootTimeMax = attack.MaxShootTime;
                shootTimeMin = attack.MinShootTime;
                waitingBeforeShoot = attack.WaitingBeforeShoot;
                waitingAfterShoot = attack.WaitingAfterShoot;

                hasGeted = true;

            }
        }

    }


    public void Apply(GameObject g)
    {
        if (edit)
        {
            movement = g.GetComponentInChildren<EnemyAxisMovement>();
            attack = g.GetComponentInChildren<EnemyAttack>();

            if (movement == null || attack == null) return;

            movement.MaxSpeed = speedMax;
            movement.MinSpeed = speedMin;
            movement.ChangeSpeed = changeSpeed;
            movement.MaxChangeTime = changeTimeMax;
            movement.MinChangeTime = changeTimeMin;

            attack.AttackColor = attakColor;

            attack.EnemyType = GetEnemyType();

            attack.IsKamikaze = isKamikaze;
            attack.MaxKamikazeTime = kamikazeTimeMax;
            attack.MinKamikazeTime = kamikazeTimeMin;
            attack.KamikazeSpeed = kamikazeSpeed;

            attack.FireBullets = fireBullets;
            attack.MaxShootTime = shootTimeMax;
            attack.MinShootTime = shootTimeMin;
            attack.WaitingBeforeShoot = waitingBeforeShoot;
            attack.WaitingAfterShoot = waitingAfterShoot;
            attack.ValuePoints = pointsForKilling;

            if (isKamikaze)
            {
                Vector3 pos = attack.gameObject.transform.position;
                attack.gameObject.transform.position = new Vector3(pos.x, pos.y, 18f);
            }
        }
        

    }

    private int GetEnemyType()
    {
        if (enemy.name == "Enemy1") return 1;
        if (enemy.name == "Enemy2") return 2;
        if (enemy.name == "Enemy3") return 3;
        if (enemy.name == "Enemy4") return 4;
        if (enemy.name == "Enemy5") return 5;
        if (enemy.name == "Enemy6") return 6;

        return 0;
    }


}
