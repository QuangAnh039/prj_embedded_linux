import React, { useState, useEffect } from 'react';
import { Box, Typography, Paper } from '@mui/material';

export default function DataSensor() {
  const [sensorData, setSensorData] = useState({
    temperature: 'Loading...',
    humidity: 'Loading...',
    light: 'Loading...',
  });

  useEffect(() => {
    setTimeout(() => {
      setSensorData({
        temperature: '25°C',
        humidity: '60%',
        light: '300 lx',
      });
    }, 2000);
  }, []);

  return (
    <Box padding="20px" marginLeft="250px">
      <Paper elevation={3} style={{ padding: '20px' }}>
        <Typography variant="h4" gutterBottom>
          Data Sensor
        </Typography>
        <Typography variant="body1">Temperature: {sensorData.temperature}</Typography>
        <Typography variant="body1">Humidity: {sensorData.humidity}</Typography>
        <Typography variant="body1">Light: {sensorData.light}</Typography>
      </Paper>
    </Box>
  );
}
