
def write_bool_msg(t, topic, msg, csv_writer):        
    csv_writer.writerow([t, topic, int(msg.data)])

def write_twist_msg(t, topic, msg, csv_writer):        
    csv_writer.writerow([t, topic+'/linear/x', msg.linear.x])
    csv_writer.writerow([t, topic+'/linear/y', msg.linear.y])
    csv_writer.writerow([t, topic+'/linear/z', msg.linear.z])
    csv_writer.writerow([t, topic+'/angular/x', msg.angular.x])
    csv_writer.writerow([t, topic+'/angular/y', msg.angular.y])
    csv_writer.writerow([t, topic+'/angular/z', msg.angular.z])
    
def write_fix_msg(t, topic, msg, csv_writer):        
    csv_writer.writerow([t, topic+'/latitude', msg.latitude])
    csv_writer.writerow([t, topic+'/longitude', msg.longitude])
    csv_writer.writerow([t, topic+'/altitude', msg.altitude])
    csv_writer.writerow([t, topic+'/status', msg.status.status])
    
def write_imu_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/orientation/w', msg.orientation.w])
    csv_writer.writerow([t, topic+'/orientation/x', msg.orientation.x])
    csv_writer.writerow([t, topic+'/orientation/y', msg.orientation.y])
    csv_writer.writerow([t, topic+'/orientation/z', msg.orientation.z])
    csv_writer.writerow([t, topic+'/linear_acceleration/x', msg.linear_acceleration.x])
    csv_writer.writerow([t, topic+'/linear_acceleration/y', msg.linear_acceleration.y])
    csv_writer.writerow([t, topic+'/linear_acceleration/z', msg.linear_acceleration.z])
    csv_writer.writerow([t, topic+'/angular_velocity/x', msg.angular_velocity.x])
    csv_writer.writerow([t, topic+'/angular_velocity/y', msg.angular_velocity.y])
    csv_writer.writerow([t, topic+'/angular_velocity/z', msg.angular_velocity.z])

def write_throttle_report_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/pedal_input', msg.pedal_input])
    csv_writer.writerow([t, topic+'/pedal_cmd', msg.pedal_cmd])
    csv_writer.writerow([t, topic+'/pedal_output', msg.pedal_output])
    csv_writer.writerow([t, topic+'/enabled', int(msg.enabled)])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_ch1', int(msg.fault_ch1)])
    csv_writer.writerow([t, topic+'/fault_ch2', int(msg.fault_ch2)])
    csv_writer.writerow([t, topic+'/fault_connector', int(msg.fault_connector)])
    
def write_brake_report_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/pedal_input', msg.pedal_input])
    csv_writer.writerow([t, topic+'/pedal_cmd', msg.pedal_cmd])
    csv_writer.writerow([t, topic+'/pedal_output', msg.pedal_output])
    csv_writer.writerow([t, topic+'/torque_input', msg.torque_input])
    csv_writer.writerow([t, topic+'/torque_cmd', msg.torque_cmd])
    csv_writer.writerow([t, topic+'/torque_output', msg.torque_output])
    csv_writer.writerow([t, topic+'/boo_input', int(msg.boo_input)])
    csv_writer.writerow([t, topic+'/boo_cmd', int(msg.boo_cmd)])
    csv_writer.writerow([t, topic+'/boo_output', int(msg.boo_output)])
    csv_writer.writerow([t, topic+'/enabled', int(msg.enabled)])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
   
def write_steering_report_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/steering_wheel_angle', msg.steering_wheel_angle])
    csv_writer.writerow([t, topic+'/steering_wheel_angle_cmd', msg.steering_wheel_angle_cmd])
    csv_writer.writerow([t, topic+'/steering_wheel_torque', msg.steering_wheel_torque])
    csv_writer.writerow([t, topic+'/speed', msg.speed])
    csv_writer.writerow([t, topic+'/enabled', int(msg.enabled)])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_bus1', int(msg.fault_bus1)])
    csv_writer.writerow([t, topic+'/fault_bus2', int(msg.fault_bus2)])
    csv_writer.writerow([t, topic+'/fault_connector', int(msg.fault_connector)])
    
def write_gear_report_msg(t, topic, msg, csv_writer):  
    csv_writer.writerow([t, topic+'/state', msg.state.gear])
    csv_writer.writerow([t, topic+'/cmd', msg.cmd.gear])
    csv_writer.writerow([t, topic+'/driver', int(msg.driver)])
    csv_writer.writerow([t, topic+'/fault_bus', int(msg.fault_bus)])
  
def write_fuel_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/fuel_level', msg.fuel_level])
  
def write_tire_pressure_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/front_left', msg.front_left])
    csv_writer.writerow([t, topic+'/front_right', msg.front_right])
    csv_writer.writerow([t, topic+'/rear_left', msg.rear_left])
    csv_writer.writerow([t, topic+'/rear_right', msg.rear_right])

def write_wheel_speed_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/front_left', msg.front_left])
    csv_writer.writerow([t, topic+'/front_right', msg.front_right])
    csv_writer.writerow([t, topic+'/rear_left', msg.rear_left])
    csv_writer.writerow([t, topic+'/rear_right', msg.rear_right])
    
def write_misc1_msg(t, topic, msg, csv_writer):
    csv_writer.writerow([t, topic+'/turn_signal', msg.turn_signal.turn_signal])
    csv_writer.writerow([t, topic+'/high_beam_headlights', int(msg.high_beam_headlights)])
    csv_writer.writerow([t, topic+'/wiper', msg.wiper.wiper])
    csv_writer.writerow([t, topic+'/ambient_light', msg.ambient_light.ambient_light])
    csv_writer.writerow([t, topic+'/btn_cc_on_off', int(msg.btn_cc_on_off)])
    csv_writer.writerow([t, topic+'/btn_cc_res_cncl', int(msg.btn_cc_res_cncl)])
    csv_writer.writerow([t, topic+'/btn_cc_set_inc', int(msg.btn_cc_set_inc)])
    csv_writer.writerow([t, topic+'/btn_cc_set_dec', int(msg.btn_cc_set_dec)])
    csv_writer.writerow([t, topic+'/btn_cc_gap_inc', int(msg.btn_cc_gap_inc)])
    csv_writer.writerow([t, topic+'/btn_cc_gap_dec', int(msg.btn_cc_gap_dec)])
    csv_writer.writerow([t, topic+'/btn_la_on_off', int(msg.btn_la_on_off)])
    csv_writer.writerow([t, topic+'/fault_bus', int(msg.fault_bus)])