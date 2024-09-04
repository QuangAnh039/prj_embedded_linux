import React, { useState } from 'react';
import { Box, Typography, TextField, Button, Paper } from '@mui/material';

export default function Profile() {
  const [name, setName] = useState('Nguyen Quang Anh');
  const [email, setEmail] = useState('quanganhn042@gmail.com');

  const handleNameChange = (e) => setName(e.target.value);
  const handleEmailChange = (e) => setEmail(e.target.value);

  const handleSave = () => {
    alert('Profile updated successfully!');
  };

  return (
    <Box padding="20px" marginLeft="250px">
      <Paper elevation={3} style={{ padding: '20px' }}>
        <Typography variant="h4" gutterBottom>
          Profile
        </Typography>
        <form>
          <Box marginBottom="20px">
            <TextField
              fullWidth
              label="Name"
              variant="outlined"
              value={name}
              onChange={handleNameChange}
            />
          </Box>
          <Box marginBottom="20px">
            <TextField
              fullWidth
              label="Email"
              variant="outlined"
              value={email}
              onChange={handleEmailChange}
            />
          </Box>
          <Button variant="contained" color="primary" onClick={handleSave}>
            Save
          </Button>
        </form>
      </Paper>
    </Box>
  );
}
