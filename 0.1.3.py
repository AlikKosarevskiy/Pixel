#Working demo project
import paho.mqtt.client as mqtt
import time

# MQTT broker settings
mqtt_broker = "192.168.43.151"
mqtt_port = 1883

# Create an MQTT client
client = mqtt.Client()

# Number of iterations
num_iterations = 3

# Variable to track the sum of durations
total_duration = 0

# Expected number of duration messages to receive
expected_durations = num_iterations

# Callback function for when a message is received
def on_message(client, userdata, message):
    global total_duration, expected_durations
    topic = message.topic
    payload = message.payload.decode('utf-8')
    print(f"Received message on topic '{topic}': {payload}")

    # Check if the message was received on the "Node1" topic
    if topic == "Node1":
        try:
            duration = int(payload)
            total_duration += duration
            expected_durations -= 1
            print(f"Duration: {duration} ms")
        except ValueError:
            print("Invalid duration format")

# Set the on_message callback
client.on_message = on_message

# Connect to the MQTT broker
client.connect(mqtt_broker, mqtt_port, 60)

try:
    for i in range(1, num_iterations + 1):
        # Publish message "i" to the "Game" topic
        client.publish("Game", str(i))
        print(f"Sent message {i} to Game")

        # Wait for a message on the "Node1" topic
        client.subscribe("Node1")
        expected_durations = 1  # Reset the expected_durations
        while expected_durations > 0:
            client.loop()

        # Reset total_duration for the next iteration
        #total_duration = 0

        # Wait for 3 seconds before sending the next message
        time.sleep(3)

except KeyboardInterrupt:
    client.disconnect()

# Display the total duration after all iterations
print(f"Total Duration: {total_duration} ms")
