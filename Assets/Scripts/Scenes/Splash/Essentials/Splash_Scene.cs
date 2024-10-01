using UnityEngine;
using UnityEngine.SceneManagement;

public class Splash_Scene : BaseScene
{
    public override void InitializeSpecifics()
    {
        Application.targetFrameRate = DB.Config.targetFrameRate;
        DB.MqttClient.Connect();

        SceneManager.LoadScene("1_Prototype");
    }
}
