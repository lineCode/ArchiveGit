<?php
	// The facebook client library
	include_once 'facebook-platform/php4client/facebook.php';

	// This defines the basic setup
	include_once 'config.php';
	include_once 'facebook.lib.php';
	include_once 'dump.php';

	$pageName = 'Mobilize Friend\'s Photos';

	$facebook = new Facebook($api_key, $secret);
	$user = $facebook->get_loggedin_user();
	$userid = (int)Param('user', $user);
	$firstName = GetUserInfo($userid, 'first_name');
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
	<title></title>
	<?php include('fbstyles.part.htm'); ?>
	<style type="text/css"><?php include('styles.css'); ?></style>
	<script type="text/javascript"><?php include('fbscript.js'); ?></script>
</head>
<body class="fbframe">
	<?php include('header.part.php'); ?>

	<div id="friendtables">
		<?php IncludeUsers(array($userid), 'friends.part.php'); ?>
		<hr/>

		<div style="width:100%; height:16px; font:bold 11pt Arial; text-align:center;">
			<?= $firstName ?> has tagged the following people:
		</div>
		<br/>
		<?php IncludeUsers(GetTaggedByUserArray($userid), 'friends.part.php'); ?>
		<hr/>

		<div style="width:100%; height:16px; font:bold 11pt Arial; text-align:center;">
			<?= $firstName ?> has been tagged BY the following people:
		</div>
		<br/>
		<?php IncludeUsers(GetWhoTaggedUserArray($userid), 'friends.part.php'); ?>
		<hr/>

		<div style="width:100%; height:16px; font:bold 11pt Arial; text-align:center;">
			<?= $firstName ?> has been tagged WITH the following people:
		</div>
		<br/>
		<?php IncludeUsers(GetTaggedWithUserArray($userid), 'friends.part.php'); ?>
	</div>

	<div style="position: relative; bottom: 0;">
		<?php include('footer.part.php'); ?>
	</div>
</body>
</html>