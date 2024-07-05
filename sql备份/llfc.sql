/*
 Navicat Premium Data Transfer

 Source Server         : llfcmysql
 Source Server Type    : MySQL
 Source Server Version : 80027 (8.0.27)
 Source Host           : 81.68.86.146:3308
 Source Schema         : llfc

 Target Server Type    : MySQL
 Target Server Version : 80027 (8.0.27)
 File Encoding         : 65001

 Date: 05/07/2024 12:01:08
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for friend
-- ----------------------------
DROP TABLE IF EXISTS `friend`;
CREATE TABLE `friend`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `self_id` int NOT NULL,
  `friend_id` int NOT NULL,
  `back` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT '',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `self_friend`(`self_id` ASC, `friend_id` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of friend
-- ----------------------------

-- ----------------------------
-- Table structure for friend_apply
-- ----------------------------
DROP TABLE IF EXISTS `friend_apply`;
CREATE TABLE `friend_apply`  (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `from_uid` int NOT NULL,
  `to_uid` int NOT NULL,
  `status` smallint NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `from_uid`(`from_uid` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 26 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of friend_apply
-- ----------------------------
INSERT INTO `friend_apply` VALUES (5, 1019, 1002, 1);
INSERT INTO `friend_apply` VALUES (6, 1023, 1002, 1);
INSERT INTO `friend_apply` VALUES (25, 0, 1023, 0);

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `uid` int NOT NULL DEFAULT 0,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `email` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `pwd` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `nick` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `desc` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `sex` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `uid`(`uid` ASC) USING BTREE,
  UNIQUE INDEX `email`(`email` ASC) USING BTREE,
  INDEX `name`(`name` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 27 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES (3, 1002, 'llfc', 'secondtonone1@163.com', '654321)', '', '', 0);
INSERT INTO `user` VALUES (4, 1003, 'tc', '18165031775@qq.com', '123456', '', '', 0);
INSERT INTO `user` VALUES (5, 1004, 'yuanweihua', '1456188862@qq.com', '}kyn;89>?<', '', '', 0);
INSERT INTO `user` VALUES (6, 1005, 'test', '2022202210033@whu.edu.cn', '}kyn;89>?<', '', '', 0);
INSERT INTO `user` VALUES (7, 1006, 'q', 'w', 'e', '', '', 0);
INSERT INTO `user` VALUES (8, 1007, 'fhr', '3157199927@qq.com', 'xuexi1228', '', '', 0);
INSERT INTO `user` VALUES (9, 1008, 'zglx2008', 'zglx2008@163.com', '123456', '', '', 0);
INSERT INTO `user` VALUES (13, 1012, 'resettest', '1042958553@qq.com', '230745', '', '', 0);
INSERT INTO `user` VALUES (14, 1013, 'rss_test', '1685229455@qq.com', '123456', '', '', 0);
INSERT INTO `user` VALUES (15, 1014, '123456789', '1152907774@qq.com', '123456789', '', '', 0);
INSERT INTO `user` VALUES (16, 1015, 'aaaaaaa', '3031719794@qq.com', '777777', '', '', 0);
INSERT INTO `user` VALUES (17, 1016, 'aaa', '2996722319@qq.com', '222222', '', '', 0);
INSERT INTO `user` VALUES (20, 1019, 'zack', '1017234088@qq.com', '654321)', '', '', 0);
INSERT INTO `user` VALUES (21, 1020, 'aatext', '1584736136@qq.com', '745230', '', '', 0);
INSERT INTO `user` VALUES (22, 1021, 'ferrero1', '1220292901@qq.com', '1234', '', '', 0);
INSERT INTO `user` VALUES (23, 1022, 'ferrero2', '15504616642@163.com', '1234', '', '', 0);
INSERT INTO `user` VALUES (24, 1023, 'lyf', '3194811890@qq.com', '123456', '', '', 0);
INSERT INTO `user` VALUES (25, 1024, 'lh', '2494350589@qq.com', 'fb8::>:;8<', '', '', 0);
INSERT INTO `user` VALUES (26, 1025, 'jf', 'luojianfeng553@163.com', '745230', '', '', 0);

-- ----------------------------
-- Table structure for user_id
-- ----------------------------
DROP TABLE IF EXISTS `user_id`;
CREATE TABLE `user_id`  (
  `id` int NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of user_id
-- ----------------------------
INSERT INTO `user_id` VALUES (1025);

-- ----------------------------
-- Procedure structure for reg_user
-- ----------------------------
DROP PROCEDURE IF EXISTS `reg_user`;
delimiter ;;
CREATE PROCEDURE `reg_user`(IN `new_name` VARCHAR(255), 
    IN `new_email` VARCHAR(255), 
    IN `new_pwd` VARCHAR(255), 
    OUT `result` INT)
BEGIN
    -- 如果在执行过程中遇到任何错误，则回滚事务
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        -- 回滚事务
        ROLLBACK;
        -- 设置返回值为-1，表示错误
        SET result = -1;
    END;
    -- 开始事务
    START TRANSACTION;
    -- 检查用户名是否已存在
    IF EXISTS (SELECT 1 FROM `user` WHERE `name` = new_name) THEN
        SET result = 0; -- 用户名已存在
        COMMIT;
    ELSE
        -- 用户名不存在，检查email是否已存在
        IF EXISTS (SELECT 1 FROM `user` WHERE `email` = new_email) THEN
            SET result = 0; -- email已存在
            COMMIT;
        ELSE
            -- email也不存在，更新user_id表
            UPDATE `user_id` SET `id` = `id` + 1;
            -- 获取更新后的id
            SELECT `id` INTO @new_id FROM `user_id`;
            -- 在user表中插入新记录
            INSERT INTO `user` (`uid`, `name`, `email`, `pwd`) VALUES (@new_id, new_name, new_email, new_pwd);
            -- 设置result为新插入的uid
            SET result = @new_id; -- 插入成功，返回新的uid
            COMMIT;
        END IF;
    END IF;
END
;;
delimiter ;

SET FOREIGN_KEY_CHECKS = 1;
