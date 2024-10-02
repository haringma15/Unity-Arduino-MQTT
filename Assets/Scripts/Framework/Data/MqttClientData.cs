using System;
using System.Text;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using UnityEngine;
using Random = UnityEngine.Random;

public class MqttClientData : MonoBehaviour
{
    public MqttClient mqttClient;


    public void Connect()
    {
        try
        {
            mqttClient = new MqttClient(Secret.MqttHost, Secret.MqttPort, false, null, null, MqttSslProtocols.None);
            mqttClient.MqttMsgPublishReceived += OnMqttMessageReceived;

            string clientID = $"TestClient_{Random.Range(10000, 99999)}";
            mqttClient.Connect(clientID, Secret.MqttUsername, Secret.MqttPassword);

            Subscribe(DB.Helpers.testTopic);

            Debug.Log($"Client {clientID} is now connected? {mqttClient.IsConnected}");
        }
        catch (Exception exception)
        {
            Debug.Log(exception);
        }
    }

    public void Subscribe(string topic)
    {
        string[] topics = new string[] { topic };
        byte[] qosLevels = new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE };
        mqttClient.Subscribe(topics, qosLevels);
    }

    public void CleanupSession()
    {
        string[] topics = new string[] { DB.Helpers.testTopic };
        mqttClient.Unsubscribe(topics);
    }

    public void Publish(string topic, string json) => Publish(topic, Encoding.UTF8.GetBytes(json));
    public void Publish(string topic, byte[] message)
    {
        if (string.IsNullOrEmpty(topic)) return;

        Debug.Log($"Publish message {message} to topic {topic}");
        mqttClient.Publish(topic, message);
    }


    public void OnDestroy()
    {
        if (mqttClient != null && mqttClient.IsConnected) mqttClient.Disconnect();
    }


    private void OnMqttMessageReceived(object sender, MqttMsgPublishEventArgs e)
    {
        Debug.Log($"received message {e.Message} for topic {e.Topic}");
        switch (e.Topic.Split('-')[0])
        {
            case "Arduino":
                MainThreadDispatcher.RunOnMainThread(
                    () => ((Prototype_Scene)DB.Helpers.currentScene).IncrementCounter()
                );
            break;
        }
    }
}
